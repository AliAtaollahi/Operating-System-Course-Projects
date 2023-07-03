#pragma once

#include "definitions.h"
#include "consts.h"

void addAdvertising(int port,Advertising adds[],int* sizeOfAdds,char* sellerName,fd_set* all_set,int* max_sd,
                    Request requests[],int sizeOfRequests);
void showList(Advertising adds[],int sizeOfAdds);
void answerRequest(int portOfUdp,Advertising adds[],int sizeOfAdds,Request requests[],int sizeOfRequests);
void listenToTcpPort(int* i,char buffer[MAX_BUFF_SIZE],fd_set* all_set,Request requests[],int* sizeOfRequests,
                    Advertising adds[],int sizeOfAdds,int udpPort);
void showRequests(Request requests[],int sizeOfRequests);
void sendListUpdatedMessage(int port,Advertising adds[],int sizeOfAdds);
void noResponse(Advertising adds[],int addIndex,Request requests[],int sizeOfRequests);