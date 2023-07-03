#include "../includes/server.h"
#include "../includes/consts.h"
#include "../includes/messages.h"
#include "../includes/utils.h"
#include "../includes/commands.h"
#include "../includes/dashboard.h"

void getSellerName(char sellerName[]) {
    memset(sellerName,0,MAX_BUFF_SIZE);
    write(STD_OUT,LOGIN_MESSAGE,strlen(LOGIN_MESSAGE));
    read(STD_IN,sellerName,MAX_BUFF_SIZE);
    sellerName[strlen(sellerName)-1] = '\0';
}

void mainMenu(Advertising adds[],int* sizeOfAdds,Request requests[],int* sizeOfRequests,int udpPort,
              char sellerName[],fd_set* all_set,int* max_sd,int* checkListen) {
    *checkListen = 0;
    char cmd[MAX_BUFF_SIZE];
    memset(cmd, 0, MAX_BUFF_SIZE);
    read(STD_IN,cmd,MAX_BUFF_SIZE);
    if(strcmp(cmd,"help\n") == 0) write(STD_OUT,HELP_MESSAGE,strlen(HELP_MESSAGE));
    else if(strcmp(cmd,"add_advertising\n") == 0) addAdvertising(udpPort,adds,sizeOfAdds,sellerName,all_set,max_sd,
                                                                requests,*sizeOfRequests);
    else if(strcmp(cmd,"show_list\n") == 0) showList(adds,*sizeOfAdds);
    else if(strcmp(cmd,"show_requests\n") == 0) showRequests(requests,*sizeOfRequests);
    else if(strcmp(cmd,"answer_request\n") == 0) answerRequest(udpPort,adds,*sizeOfAdds,requests,*sizeOfRequests);
    else if(strcmp(cmd,"clear\n") == 0) system("clear");
    else write(STD_OUT,INIT_COMMAND,strlen(INIT_COMMAND));
}

void checkPorts(Advertising adds[],int* sizeOfAdds,int i,int* checkListen,fd_set* all_set,int* max_sd,int udpPort) {
    int new_socket;
    for (int j = 0; j < *sizeOfAdds; j++)
        if (i == adds[j].socket) {
            *checkListen = 0;
            int temp = new_socket;
            new_socket = acceptClient(adds[j].socket);
            if (adds[j].state == WAITING) {
                FD_SET(new_socket, all_set);
                if (new_socket > *max_sd)
                    *max_sd = new_socket;
                adds[j].state = IN_PROGRESS;
                adds[j].acceptClientFd = new_socket;
                sendListUpdatedMessage(udpPort,adds,*sizeOfAdds);
            }
            else {
                send(new_socket, CLOSE_CONNECTION_MESSAGE, strlen(CLOSE_CONNECTION_MESSAGE), 0);
                new_socket = temp;
            }
        }
}

void dashboard(char const *argv[]) {
    int checkListen = 1,udpPort = atoi(argv[PORT_ORDER_IN_ARGV]);
    Advertising adds[MAX_ADVERTISING];
    Request requests[MAX_REQUEST];
    int sizeOfAdds = 0,sizeOfRequests = 0;
    
    char sellerName[MAX_BUFF_SIZE], buffer[MAX_BUFF_SIZE] = {0};
    getSellerName(sellerName);

    fd_set all_set, read_fds;
    int max_sd = STD_IN;
    FD_ZERO(&all_set);
    FD_SET(STD_IN, &all_set);

    write(STD_OUT,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));

    while (1) {
        read_fds = all_set;
        select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == STD_IN) mainMenu(adds,&sizeOfAdds,requests,&sizeOfRequests,udpPort,sellerName,&all_set,&max_sd,&checkListen) ;
                checkPorts(adds,&sizeOfAdds,i,&checkListen,&all_set,&max_sd,udpPort);
                if (checkListen) listenToTcpPort(&i,buffer,&all_set,requests,&sizeOfRequests,adds,sizeOfAdds,udpPort);
                checkListen = 1;
            }
        }
    }
}