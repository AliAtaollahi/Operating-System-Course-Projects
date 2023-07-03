#pragma once

#include "consts.h"

typedef struct Advertising_Announcement
{
    char name[MAX_BUFF_SIZE];
    char* sellerName;
    int port;
    int price;
    int state;
    int socket;
    int acceptClientFd;
} Advertising;

typedef struct Tcp_Request
{
    int port;
    int price;
    int state;
    int socket;
    char name[MAX_BUFF_SIZE];
} Request;
