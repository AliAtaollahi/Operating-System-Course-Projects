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
#include <signal.h>
#include "consts.h"

int setupServer(int port);
int acceptClient(int server_fd);
void setUdpSocket(int* sock,struct sockaddr_in* bc_address,int port);
int connectServer(int port);