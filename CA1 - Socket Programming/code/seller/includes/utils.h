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
#include "definitions.h"
#include "consts.h"

void putAdvertisingInOneLineForBuyer(Advertising add,char* buffer);
char* putAdvertisingInOneLineForSeller(Advertising add,char* buffer);
void putRequestInOneLineForSeller(Request request,char* buff);
char* intToStr(int num);
void addAdvertisingToList(Advertising adds[],int* sizeOfAdds,char* sellerName);
void makeListForBuyer(Advertising adds[],char* buffer,int sizeOfAdds);