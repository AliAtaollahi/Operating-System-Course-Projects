#include "../includes/server.h"
#include "../includes/consts.h"
#include "../includes/messages.h"
#include "../includes/utils.h"
#include "../includes/commands.h"
#include "../includes/dashboard.h"

void runUdpServer(int* udpSock,int port) {
    int broadcast = 1, opt = 1;
    struct sockaddr_in bc_address;
    int udpPort = port;

    *udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(*udpSock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(*udpSock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    bc_address.sin_family = AF_INET; 
    bc_address.sin_port = htons(udpPort); 
    bc_address.sin_addr.s_addr = inet_addr(INET_ADDRESS);

    bind(*udpSock, (struct sockaddr *)&bc_address, sizeof(bc_address));

}

void getBuyerName(char buyerName[]) {
    memset(buyerName,0,MAX_BUFF_SIZE);
    write(STD_OUT,LOGIN_MESSAGE,strlen(LOGIN_MESSAGE));
    read(STD_IN,buyerName,MAX_BUFF_SIZE);
    buyerName[strlen(buyerName)-1] = '\0';
}
void makeEndTcpConnectionSentence(char buffer[],int tcpPort,int fd) {
    strcpy(buffer,"#");
    strcat(buffer,intToStr(tcpPort));
    send(fd,buffer,strlen(buffer),0);
}
void waitingForResponse(int fd,char bufferTcp[],int tcpPort,int* waitedForResponse,fd_set* all_set) {
    int bytes_received = 0;   
    alarm(TIME_OUT_LIMIT);
    bytes_received = read(fd , bufferTcp, MAX_BUFF_SIZE);
    alarm(0);

    char buffer[MAX_BUFF_SIZE] = {0};
    makeEndTcpConnectionSentence(buffer,tcpPort,fd);
    *waitedForResponse = 0;

    if (bytes_received == 0) {}
    else if (strcmp(bufferTcp,CLOSE_CONNECTION_MESSAGE) == 0) printf("port is in_progress or selled or closed\n\n");
    else if (strcmp(bufferTcp,POSITIVE_TO_REQUEST_MESSAGE) == 0) printf("Request Accepted\n\n");
    else if (strcmp(bufferTcp,NEGATIVE_TO_REQUEST_MESSAGE) == 0) printf("Request Denied\n\n");
    close(fd);
    FD_CLR(fd, all_set);
}

void mainMenu(char sellerList[MAX_ADVERTISING][MAX_BUFF_SIZE],int size,int* fd,int* max_sd,fd_set* all_set
             ,int* waitedForResponse,int* tcpPort) {
    char cmd[MAX_BUFF_SIZE];
    memset(cmd, 0, MAX_BUFF_SIZE);
    read(STD_IN,cmd,MAX_BUFF_SIZE);
    if(strcmp(cmd,"help\n") == 0) write(STD_OUT,HELP_MESSAGE,strlen(HELP_MESSAGE));
    else if(strcmp(cmd,"show_list\n") == 0) printList(sellerList,size);
    else if(strcmp(cmd,"send_request\n") == 0) sendRequest(fd,max_sd,all_set,waitedForResponse,tcpPort);
    else if(strcmp(cmd,"clear\n") == 0) system("clear");
}

void udpListening(char bufferUdp[],int udpSock,char sellerList[MAX_ADVERTISING][MAX_BUFF_SIZE],int* size) {
    memset(bufferUdp, 0, MAX_BUFF_SIZE);
    recv(udpSock, bufferUdp, MAX_BUFF_SIZE, 0);
    updateList(sellerList,bufferUdp,size);
    announceNewAdvertising(bufferUdp);
}

void dashboard (char const *argv[]) {
    signal(SIGALRM, alarmHandler);
    siginterrupt(SIGALRM, 1);

    int waitedForResponse = 0,size = 0,tcpPort=0,udpSock;
    char buyerName[MAX_BUFF_SIZE];
    getBuyerName(buyerName);

    char sellerList[MAX_ADVERTISING][MAX_BUFF_SIZE] = {0},bufferUdp[MAX_BUFF_SIZE] = {0},bufferTcp[MAX_BUFF_SIZE] = {0};

    runUdpServer(&udpSock,atoi(argv[PORT_ORDER_IN_ARGV]));

    int clientUdpFd = udpSock,max_sd = udpSock,fd;
    fd_set all_set, read_fds;
    FD_ZERO(&all_set);
    FD_SET(clientUdpFd, &all_set);
    FD_SET(STD_IN, &all_set);

    write(STD_OUT,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));

    while (1) {
        read_fds = all_set;
        select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if(i == udpSock) udpListening(bufferUdp,udpSock,sellerList,&size);
                else if (i == STD_IN) mainMenu(sellerList,size,&fd,&max_sd,&all_set,&waitedForResponse,&tcpPort);
            }
            if(waitedForResponse) waitingForResponse(fd,bufferTcp,tcpPort,&waitedForResponse,&all_set);
        }

    }
}