#include "../includes/server.h"
#include "../includes/messages.h"
#include "../includes/consts.h"

int setupServer(int port) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    listen(server_fd, LISTEN_NUM);

    return server_fd;
}

int acceptClient(int server_fd) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

    return client_fd;
}

void setUdpSocket(int* sock,struct sockaddr_in* bc_address,int port) {
    int broadcast = 1, opt = 1;
    char buffer[MAX_BUFF_SIZE] = {0};
    *sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(*sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(*sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    bc_address->sin_family = AF_INET; 
    bc_address->sin_port = htons(port); 
    bc_address->sin_addr.s_addr = inet_addr(INET_ADDRESS);

    bind(*sock, (struct sockaddr *)&bc_address, sizeof(bc_address));
}

int connectServer(int port) {
    int fd;
    struct sockaddr_in server_address;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(port); 
    server_address.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
        printf(ERROR_IN_CONNECTION_MESSAGE);
    }

    return fd;
}
