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
#include "server.h"
#include "consts.h"
#include "messages.h"

void printList(char sellerList[][MAX_BUFF_SIZE],int size);
void sendRequest(int* fd,int* max_sd,fd_set* all_set,int* waitedForResponse,int* tcpPort);