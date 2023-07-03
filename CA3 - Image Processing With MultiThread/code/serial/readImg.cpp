#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <math.h>

using namespace std;

#pragma pack(1)
#pragma once
#define FULL_COLOR 255

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct bits {
    unsigned char red, green, blue;
} Pixel;

vector<vector<Pixel>> img, img_copy;

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
    vector<vector<Pixel> > raw_table(rows, vector<Pixel>(cols, {0, 0, 0}));
    img = raw_table;
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
                        img[i][j].red   = fileReadBuffer[end - count++];
                        break;
                    case 1:
                        img[i][j].green = fileReadBuffer[end - count++];
                        break;
                    case 2:
                        img[i][j].blue  = fileReadBuffer[end - count++];
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void flip(int rows, int cols)
{
    for (int i = 0; i < rows; i++)
        reverse(img[i].begin(), img[i].end());
}

int modulo(int value, int m) 
{
    int mod = value % (int)m;
    if (mod < 0) {
        mod += m;
    }
    return mod;
}

int fix_res(int res)
{
    if(res > FULL_COLOR)
        res = FULL_COLOR;
    else if (res < 0)
        res = 0;
    return res;
}

void emboss(int rows, int cols)
{
    img_copy = img;
    int res;
    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            pair<int, int> d1 = {modulo(i - 1, rows), modulo(j - 1, cols)},
                           d2 = {modulo(i - 1, rows), modulo(j, cols)},
                           d3 = {modulo(i - 1, rows), modulo(j + 1, cols)}, 
                           d4 = {modulo(i, rows), modulo(j - 1, cols)}, 
                           d5 = {modulo(i, rows), modulo(j, cols)}, 
                           d6 = {modulo(i, rows), modulo(j + 1, cols)}, 
                           d7 = {modulo(i + 1, rows), modulo(j - 1, cols)}, 
                           d8 = {modulo(i + 1, rows), modulo(j, cols)}, 
                           d9 = {modulo(i + 1, rows), modulo(j + 1, cols)};

            res =   (-2 * img_copy[d1.first][d1.second].red) - img_copy[d2.first][j].red   + (0 * img_copy[d3.first][d3.second].red) -
                          img_copy[d4.first][d4.second].red  + img_copy[d5.first][j].red   +      img_copy[d6.first][d6.second].red  +
                     (0 * img_copy[d7.first][d7.second].red) + img_copy[d8.first][j].red   + (2 * img_copy[d9.first][d9.second].red) ;
            img[i][j].red = fix_res(res);

            res = (-2 * img_copy[d1.first][d1.second].green) - img_copy[d2.first][j].green + (0 * img_copy[d3.first][d3.second].green) -
                        img_copy[d4.first][d4.second].green  + img_copy[d5.first][j].green +      img_copy[d6.first][d6.second].green  +
                   (0 * img_copy[d7.first][d7.second].green) + img_copy[d8.first][j].green + (2 * img_copy[d9.first][d9.second].green) ;
            img[i][j].green = fix_res(res);

            res =  (-2 * img_copy[d1.first][d1.second].blue) - img_copy[d2.first][j].blue  + (0 * img_copy[d3.first][d3.second].blue)  -
                         img_copy[d4.first][d4.second].blue  + img_copy[d5.first][j].blue  +      img_copy[d6.first][d6.second].blue   +
                    (0 * img_copy[d7.first][d7.second].blue) + img_copy[d8.first][j].blue  + (2 * img_copy[d9.first][d9.second].blue)  ;
            img[i][j].blue = fix_res(res);
        }
        
    }

}

void add_diamond(int rows,int cols)
{
    double rows_=rows, cols_=cols;
    Pixel full_color = {FULL_COLOR, FULL_COLOR, FULL_COLOR};
    for (int i = 0; i < rows/2; i++)
        img[i][round(((double(i) / rows_ * 2)*cols_/2 + cols_/2))] = full_color;
    for (int i = 0; i < rows/2; i++)
        img[i][round((1 - (double(i) / rows_ * 2))*cols_/2)] = full_color;
    for (int i = rows/2; i < rows; i++)
        img[i][round(((double(i)-(rows_/2)) / rows_ * 2)*cols_/2)] = full_color;
    for (int i = rows/2; i < rows; i++)
        img[i][round((1 - (double(i) / rows * 2)) * cols_/2 + cols_)] = full_color;
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
                        fileBuffer[bufferSize - count] = img[i][j].red;
                        count++;
                        break;
                    case 1:
                        fileBuffer[bufferSize - count] = img[i][j].green;
                        count++;
                        break;
                    case 2:
                        fileBuffer[bufferSize - count] = img[i][j].blue;
                        count++;
                        break;
                    default:
                        break;
                }
            }
    }
    write.write(fileBuffer, bufferSize);
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

    getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);
    flip(rows, cols);
    emboss(rows, cols);
    add_diamond(rows, cols);
    writeOutBmp24(fileBuffer, "output.bmp", bufferSize);

    auto end = chrono::high_resolution_clock::now();
    cout << "Execution Time: " 
         << (double)(chrono::duration_cast<chrono::milliseconds>(end - start).count()) / 1000 
         << endl;

    return 0;
}