#include "../includes/commands.h"
#include "../includes/messages.h"

void printList(char sellerList[][MAX_BUFF_SIZE],int size) {
    printf("%s",LINE_BREAKER);
    for (int i = 0; i < size; i++) {
        printf("%s",sellerList[i]);
        if(i+1 != size) printf("\n");
    }
    printf("%s\n",LINE_BREAKER);
}

void sendRequest(int* fd,int* max_sd,fd_set* all_set,int* waitedForResponse,int* tcpPort) {
    if (!(*waitedForResponse)) {
        char port[MAX_BUFF_SIZE],price[MAX_BUFF_SIZE],request[MAX_BUFF_SIZE];
        memset(port,0,MAX_BUFF_SIZE);
        write(STD_OUT,TYPE_PORT_MESSAGE,strlen(TYPE_PORT_MESSAGE));
        read(STD_IN,port,MAX_BUFF_SIZE);
        port[strlen(port)-1] = '\0';
        *tcpPort = atoi(port);

        memset(price,0,MAX_BUFF_SIZE);
        write(STD_OUT,TYPE_PRICE_MESSAGE,strlen(TYPE_PRICE_MESSAGE));
        read(STD_IN,price,MAX_BUFF_SIZE);
        printf("\n");
        price[strlen(price)-1] = '\0';

        strcpy(request, port);
        strcat(request, REQUEST_SEPERATOR);
        strcat(request, price);
        strcat(request, END_OF_REQUEST);

        *fd = connectServer(atoi(port));
        if(*fd > *max_sd) *max_sd = *fd + 1;
        FD_SET(*fd,all_set);
        send(*fd,request,MAX_BUFF_SIZE,0);
        *waitedForResponse = 1;
    }
    else write(STD_OUT,YOU_ARE_IN_WAITING_MESSAGE,strlen(YOU_ARE_IN_WAITING_MESSAGE));
}