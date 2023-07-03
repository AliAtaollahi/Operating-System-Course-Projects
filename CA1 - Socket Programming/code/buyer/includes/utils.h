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
#include "consts.h"

void updateList(char sellerList[][MAX_BUFF_SIZE],char buffer[MAX_BUFF_SIZE],int* size);
void announceNewAdvertising(char buffer[MAX_BUFF_SIZE]);
void alarmHandler(int sig);
char* intToStr(int num);