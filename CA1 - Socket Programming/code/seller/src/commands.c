#include "../includes/server.h"
#include "../includes/commands.h"
#include "../includes/utils.h"
#include "../includes/messages.h"
#include "../includes/consts.h"
#include "time.h"

void setNewFdToAllSet(Advertising adds[],int sizeOfAdds,fd_set* all_set,int* max_sd) {
    int server_fd = adds[sizeOfAdds-1].socket;
    FD_SET(server_fd, all_set);
    if (server_fd > *max_sd) *max_sd = server_fd;
}

int checkRequestsAllDone(Request requests[],int sizeOfRequests) {
    for (int i = 0; i < sizeOfRequests; i++)
        if(requests[i].state == WAITING)
            return THERE_IS_WAITING_REQUEST;
    return CHECKED;
}

int findInAdvertisingsByPort(Advertising adds[],int sizeOfAdds,int port) {
    for (int i = 0; i < sizeOfAdds; i++)
        if (adds[i].port == port)
            return i;
    return NOT_FOUND;
}

int findWaitingRequest(Request requests[],int sizeOfRequests,int port) {
    for (int i = 0; i < sizeOfRequests; i++)
        if (requests[i].port == port && requests[i].state == WAITING)
            return i;
    return NOT_FOUND;
}


void sendListUpdatedMessage(int udpPort,Advertising adds[],int sizeOfAdds) {
    int sock;
    struct sockaddr_in bc_address;
    setUdpSocket(&sock,&bc_address,udpPort);
    broadcastAdvertising(adds,sock,&bc_address,sizeOfAdds);
}

void addAdvertising(int udpPort,Advertising adds[],int* sizeOfAdds,char* sellerName,fd_set* all_set,int* max_sd,
                    Request requests[],int sizeOfRequests) {
    if(checkRequestsAllDone(requests,sizeOfRequests)) {
        addAdvertisingToList(adds,sizeOfAdds,sellerName);
        setNewFdToAllSet(adds,*sizeOfAdds,all_set,max_sd);
        sendListUpdatedMessage(udpPort,adds,*sizeOfAdds);
        write(STD_OUT,INIT_COMMAND,strlen(INIT_COMMAND));
    }
    else {
        write(STD_OUT,A_REQUEST_IS_IN_WAITING_MESSAGE,strlen(A_REQUEST_IS_IN_WAITING_MESSAGE));
    }
}

void showList(Advertising adds[],int sizeOfAdds) {
    printf("%s",LINE_BREAKER);
    write(STD_OUT,SHOW_LIST_TITLES_MESSAGE,strlen(SHOW_LIST_TITLES_MESSAGE));
    
    char buffer[MAX_BUFF_SIZE] = {0};
    for (size_t i = 0; i < sizeOfAdds ; i++) {
        memset(buffer,0,MAX_BUFF_SIZE);
        putAdvertisingInOneLineForSeller(adds[i],&buffer[0]);
        write(STD_OUT,buffer,MAX_BUFF_SIZE);
    }
    printf("%s",LINE_BREAKER);
    write(STD_OUT,INIT_COMMAND,strlen(INIT_COMMAND));
}

void listenToTcpPort(int* i,char buffer[MAX_BUFF_SIZE],fd_set* all_set,Request requests[],int* sizeOfRequests,
                    Advertising adds[],int sizeOfAdds,int udpPort) {
    char port[MAX_BUFF_SIZE],price[MAX_BUFF_SIZE];
    int bytes_received;
    bytes_received = recv(*i , buffer, MAX_BUFF_SIZE, 0);
    
    if(buffer[0] == SIGNATURE_TO_END_TCP) {
        buffer[0] = '0';
        int addIndex = findInAdvertisingsByPort(adds,sizeOfAdds,atoi(buffer));
        noResponse(adds,addIndex,requests,*sizeOfRequests);
        sendListUpdatedMessage(udpPort,adds,sizeOfAdds);
        close(*i);
        FD_CLR(*i, all_set);
        return;
    }
    if (bytes_received == 0) {
        close(*i);
        FD_CLR(*i, all_set);
        return;
    }

    int j= 0;
    for (j = 0; buffer[j] != REQUEST_SEPERATOR; j++) port[j] = buffer[j];
    j++;
    for (int index = j; buffer[j] != END_OF_REQUEST; j++) price[j - index] = buffer[j];

    Request newRequest = {atoi(port),atoi(price),WAITING,*i};
    int addIndex = findInAdvertisingsByPort(adds,sizeOfAdds,atoi(port));
    strcpy(newRequest.name,adds[addIndex].name);
    requests[*sizeOfRequests] = newRequest;
    (*sizeOfRequests)++;

    memset(buffer, 0, MAX_BUFF_SIZE);
}

void showRequests(Request requests[],int sizeOfRequests) {
    char buff[MAX_BUFF_SIZE] = {0};
    printf("%s",LINE_BREAKER);
    printf("%s",REQUEST_LIST_TITLES_MESSAGE);
    for (size_t i = 0; i < sizeOfRequests ; i++) {
        memset(buff,0,MAX_BUFF_SIZE);
        putRequestInOneLineForSeller(requests[i],&buff[0]);
        write(STD_OUT,buff,MAX_BUFF_SIZE);
    }
    printf("%s",LINE_BREAKER);
    write(STD_OUT,INIT_COMMAND,strlen(INIT_COMMAND));
}

void logResponse(Advertising add) {
    int file_fd = open(LOG_FILE, O_APPEND | O_RDWR);
    char buffer[MAX_BUFF_SIZE] = {0};
    strcpy(buffer,add.name);
    strcat(buffer," ");
    strcat(buffer,add.sellerName);
    strcat(buffer," ");
    strcat(buffer,intToStr(add.port));
    strcat(buffer," ");
    strcat(buffer,intToStr(add.price));
    write(file_fd, buffer, strlen(buffer));
    close(file_fd);
}

void yesResponse(Advertising adds[],int addIndex,Request requests[],int sizeOfRequests) {
    adds[addIndex].state = SELLED;
    send(adds[addIndex].acceptClientFd, POSITIVE_TO_REQUEST_MESSAGE, strlen(POSITIVE_TO_REQUEST_MESSAGE), 0);
    int requestIndex = findWaitingRequest(requests,sizeOfRequests,adds[addIndex].port);
    requests[requestIndex].state = DONE;
    logResponse(adds[addIndex]);
}

void noResponse(Advertising adds[],int addIndex,Request requests[],int sizeOfRequests) {
    if(adds[addIndex].state != SELLED) adds[addIndex].state = WAITING;
    send(adds[addIndex].acceptClientFd, NEGATIVE_TO_REQUEST_MESSAGE, strlen(NEGATIVE_TO_REQUEST_MESSAGE), 0);
    int requestIndex = findWaitingRequest(requests,sizeOfRequests,adds[addIndex].port);
    requests[requestIndex].state = DONE;
}

void answerRequest(int udpPort,Advertising adds[],int sizeOfAdds,Request requests[],int sizeOfRequests) {
    char port[MAX_BUFF_SIZE],response[MAX_BUFF_SIZE];
    memset(port,0,MAX_BUFF_SIZE);
    write(STD_OUT,TYPE_PORT_MESSAGE,strlen(TYPE_PORT_MESSAGE));
    read(STD_IN,port,MAX_BUFF_SIZE);
    port[strlen(port)-1] = '\0';

    memset(response,0,MAX_BUFF_SIZE);
    write(STD_OUT,TYPE_RESPONSE_MESSAGE,strlen(TYPE_RESPONSE_MESSAGE));
    read(STD_IN,response,MAX_BUFF_SIZE);
    response[strlen(response)-1] = '\0';
    write(STD_OUT,INIT_COMMAND,strlen(INIT_COMMAND));

    int addIndex = findInAdvertisingsByPort(adds,sizeOfAdds,atoi(port));
    if(strcmp(response,"yes") == 0) yesResponse(adds,addIndex,requests,sizeOfRequests);
    else if(strcmp(response,"no") == 0) noResponse(adds,addIndex,requests,sizeOfRequests);
    sendListUpdatedMessage(udpPort,adds,sizeOfAdds);
}