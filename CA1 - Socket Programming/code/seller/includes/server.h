#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h> 
#include "definitions.h"
#include "consts.h"


int setupServer(int port);
int acceptClient(int server_fd);
void broadcastAdvertising(Advertising adds[],int sock,struct sockaddr_in* bc_address,int sizeOfAdds);
void setUdpSocket(int* sock,struct sockaddr_in* bc_address,int port);