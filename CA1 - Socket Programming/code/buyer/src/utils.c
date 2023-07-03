#include "../includes/utils.h"
#include "../includes/messages.h"

int findSellerNameIndex(char sellerList[][MAX_BUFF_SIZE],char buffer[MAX_BUFF_SIZE],int size) {
    int index = NOT_FOUND;

    char str[MAX_BUFF_SIZE];
    memcpy(str,buffer,MAX_BUFF_SIZE);
    char *sellerName = strtok(str, "\n");

    for (int i = 0; i < size; i++)
    {
        char temp[MAX_BUFF_SIZE];
        memcpy(temp,sellerList[i],MAX_BUFF_SIZE);
        char *target = strtok(temp, "\n");
        if(strcmp(sellerName,target) == 0) return i;
    }

    return index;
}

void updateList(char sellerList[][MAX_BUFF_SIZE],char buffer[MAX_BUFF_SIZE],int* size) {
    int index = findSellerNameIndex(sellerList,buffer,*size);
    if(index == NOT_FOUND) {
        index = *size;
        (*size)++;
    }
    strcpy(sellerList[index],buffer);
}

void announceNewAdvertising(char buffer[MAX_BUFF_SIZE]) {
    char str[MAX_BUFF_SIZE];
    memcpy(str,buffer,MAX_BUFF_SIZE);
    char *sellerName = strtok(str, "\n");
    printf("\e\033[1;33m%s\e\033[0m \e\033[1;32mupdated\e\033[0m his/her list !\n\n",sellerName);
}

void alarmHandler(int sig) {
    write(STD_OUT,TIME_OUT_MESSAGE,strlen(TIME_OUT_MESSAGE));
}

char* intToStr(int num)
{
    char digits[] = ALL_NUMBERS;
    char temp[20];
    char* final = (char*)malloc(20);
    int i = 0, j = 0;
    do {
        temp[i++] = digits[num % 10];
        num /= 10;
    } while (num > 0);
    while(--i >= 0)
        final[j++] = temp[i];
    final[j] = '\0';
    return (char*) final;
}