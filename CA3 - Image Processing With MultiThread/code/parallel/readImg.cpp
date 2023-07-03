#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <pthread.h>

using namespace std;

#pragma pack(1)
#pragma once
#define FULL_COLOR 255

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char** COLOR;
typedef vector<vector<unsigned char>> LOCAL_COLOR;
typedef vector<unsigned char> LOCAL_PIXEL;


COLOR red_pixels, green_pixels, blue_pixels;

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

int rows;
int cols;

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
    ifstream file(fileName);

    if (file)
    {
        file.seekg(0, ios::end);
        streampos length = file.tellg();
        file.seekg(0, ios::beg);

        buffer = new char[length];
        file.read(&buffer[0], length);

        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;

        file_header = (PBITMAPFILEHEADER)(&buffer[0]);
        info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
        rows = info_header->biHeight;
        cols = info_header->biWidth;
        bufferSize = file_header->bfSize;
        return 1;
    }
    else
    {
        cout << "File" << fileName << " doesn't exist!" << endl;
        return 0;
    }
}


void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++) 
    {
        count += extra;
        for (int j = cols - 1; j >= 0; j--) 
        {
            for (int k = 0; k < 3; k++) 
            {
                switch (k) 
                {
                    case 0:
                        red_pixels[i][j]   = fileReadBuffer[end - count++];
                        break;
                    case 1:
                        green_pixels[i][j] = fileReadBuffer[end - count++];
                        break;
                    case 2:
                        blue_pixels[i][j]  = fileReadBuffer[end - count++];
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void aloc_color_pixels(COLOR& pixels)
{
    pixels = new unsigned char*[rows];
    for (int i = 0; i < rows; i++)
        pixels[i] = new unsigned char[cols];
}

void aloc_colors_pixels()
{
    aloc_color_pixels(red_pixels);
    aloc_color_pixels(green_pixels);
    aloc_color_pixels(blue_pixels);
}

void flip(int rows, int cols, COLOR& color)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols / 2; j++)
            swap(color[i][j], color[i][cols - j -1]);
}

int fix_res(int res)
{
    if(res > FULL_COLOR)
        res = FULL_COLOR;
    else if (res < 0)
        res = 0;
    return res;
}

void emboss(int rows, int cols, COLOR& color)
{
    LOCAL_COLOR color_copy = LOCAL_COLOR(rows, LOCAL_PIXEL(cols, 0));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            color_copy[i][j] = color[i][j];

    int res;
    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            res =  (-2 * color_copy[i - 1][j - 1]) - color_copy[i - 1][j] + (0 * color_copy[i - 1][j + 1]) -
                         color_copy[i][j - 1]      + color_copy[i][j]     +      color_copy[i][j + 1]      +
                    (0 * color_copy[i + 1][j - 1]) + color_copy[i + 1][j] + (2 * color_copy[i + 1][j + 1]) ;
            color[i][j] = fix_res(res);
        }
    }
}

void add_diamond(int rows,int cols, COLOR& color)
{
    double rows_=rows, cols_=cols;
    for (int i = 0; i < rows/2; i++)
        color[i][int((double(i) / rows_ * 2)*cols_/2 + cols_/2)] = FULL_COLOR;
    for (int i = 0; i < rows/2; i++)
        color[i][int((1 - (double(i) / rows_ * 2))*cols_/2)] = FULL_COLOR;
    for (int i = rows/2; i < rows; i++)
        color[i][int(((double(i)-(rows_/2)) / rows_ * 2)*cols_/2)] = FULL_COLOR;
    for (int i = rows/2; i < rows; i++)
        color[i][int((1 - (double(i) / rows * 2)) * cols_/2 + cols_)] = FULL_COLOR;
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
{
    ofstream write(nameOfFileToCreate);
    if (!write)
    {
        cout << "Failed to write " << nameOfFileToCreate << endl;
        return;
    }
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++)
    {
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                    case 0:
                        fileBuffer[bufferSize - count] = red_pixels[i][j];
                        count++;
                        break;
                    case 1:
                        fileBuffer[bufferSize - count] = green_pixels[i][j];
                        count++;
                        break;
                    case 2:
                        fileBuffer[bufferSize - count] = blue_pixels[i][j];
                        count++;
                        break;
                    default:
                        break;
                }
            }
    }
    write.write(fileBuffer, bufferSize);
}

void* call_filters(void* argument) 
{
    int* id = ((int*) argument);
    COLOR color_ = (*id == 1) ? red_pixels : (*id == 2) ? green_pixels : blue_pixels; 
    flip(rows, cols, color_);
    emboss(rows, cols, color_);
    add_diamond(rows, cols, color_);
    pthread_exit(NULL);
    return (void*) 0;
}

void apply_filters()
{
    int RED_ID = 1;
    int GREEN_ID = 2;
    int BLUE_ID = 3; 
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, call_filters, (void*) &RED_ID);
    pthread_create(&thread2, NULL, call_filters, (void*) &GREEN_ID);
    pthread_create(&thread3, NULL, call_filters, (void*) &BLUE_ID);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL); 
    pthread_join(thread3, NULL); 
}

int main(int argc, char *argv[])
{
    auto start = chrono::high_resolution_clock::now();

    char *fileBuffer;
    int bufferSize;
    char *fileName = argv[1];
    if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
    {
        cout << "File read error" << endl;
        return 1;
    }
    else aloc_colors_pixels();

    getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);

    apply_filters();

    writeOutBmp24(fileBuffer, "output.bmp", bufferSize);

    auto end = chrono::high_resolution_clock::now();
    cout << "Execution Time: " 
         << (double)(chrono::duration_cast<chrono::milliseconds>(end - start).count()) / 1000 
         << endl;

    return 0;
}