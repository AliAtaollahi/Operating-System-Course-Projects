#include "../includes/utils.h"
#include "../includes/messages.h"

const char STATES_OF_ADVERTISING[3][MAX_BUFF_SIZE] = {"\e\033[1;32mwaiting\e\033[0m",
                              "\e\033[1;33min_progress\e\033[0m",
                              "\e\033[1;31mselled\e\033[0m"};

const char STATES_OF_REQUEST[2][MAX_BUFF_SIZE] = {"\e\033[1;33mwaiting\e\033[0m",
                              "\e\033[1;32mdone\e\033[0m"};


void makeListForBuyer(Advertising adds[],char* buff,int sizeOfAdds) {
    int end = 0 ;
    strcpy(buff, adds[0].sellerName);
    strcat(buff, ADVERTISING_LIST_TITLES_MESSAGE);

    while(buff[end] != 0) end++;
    for (int i = 0; i < sizeOfAdds; i++)
    {
        putAdvertisingInOneLineForBuyer(adds[i],&buff[end]);
        while(buff[end] != 0) end++;
    }
}

void putAdvertisingInOneLineForBuyer(Advertising add,char* buff) {
    strcat(buff, add.name);
    strcat(buff, " ");
    strcat(buff, intToStr(add.price));
    strcat(buff, " ");
    strcat(buff, intToStr(add.port));
    strcat(buff, " ");
    strcat(buff, STATES_OF_ADVERTISING[add.state]);
    strcat(buff, "\n");
}

char* putAdvertisingInOneLineForSeller(Advertising add,char* buff) {
    strcat(buff, add.name);
    strcat(buff, " ");
    strcat(buff, intToStr(add.price));
    strcat(buff, " ");
    strcat(buff, intToStr(add.port));
    strcat(buff, " ");
    strcat(buff, STATES_OF_ADVERTISING[add.state]);
    strcat(buff, "\n");
}

void putRequestInOneLineForSeller(Request request,char* buff) {
    strcpy(buff, "--> ");
    strcat(buff, intToStr(request.port));
    strcat(buff, " ");
    strcat(buff, request.name);
    strcat(buff, " ");
    strcat(buff, intToStr(request.price));
    strcat(buff, " ");
    strcat(buff, STATES_OF_REQUEST[request.state]);
    strcat(buff, "\n");
}

char* intToStr(int num)
{
    char digits[] = ALL_NUMBERS;
    char temp[20];
    char* final = (char*)malloc(20);
    int i = 0, j = 0;
    do {
        temp[i++] = digits[num % 10];
        num /= 10;
    } while (num > 0);
    while(--i >= 0)
        final[j++] = temp[i];
    final[j] = '\0';
    return (char*) final;
}

int createAndRunTcpSocket(int* port) {
    struct sockaddr_in address;

    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    while (1)
    {
        *port = (rand() % (MAX_PORT - MIN_PORT + 1)) + MIN_PORT;
        address.sin_port = htons(*port);
        if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) continue;
        break;
    }    
    
    listen(server_fd, LISTEN_NUM);

    return server_fd;
}

void addAdvertisingToList(Advertising adds[],int* sizeOfAdds,char* sellerName) {
    Advertising newAdvertising;
    char temp[MAX_BUFF_SIZE];
    int price,port;

    memset(temp,0,MAX_BUFF_SIZE);
    write(STD_OUT,TYPE_NAME_MESSAGE,strlen(TYPE_PRICE_MESSAGE));
    read(STD_IN,temp,MAX_BUFF_SIZE);
    temp[strlen(temp)-1] = '\0';
    strcpy(newAdvertising.name,temp);

    memset(temp,0,MAX_BUFF_SIZE);
    write(STD_OUT,TYPE_PRICE_MESSAGE,strlen(TYPE_PRICE_MESSAGE));
    read(STD_IN,temp,MAX_BUFF_SIZE);
    price = atoi(temp);

    newAdvertising.sellerName = sellerName;
    newAdvertising.price = price;
    newAdvertising.state = WAITING;
    newAdvertising.socket = createAndRunTcpSocket(&port);
    newAdvertising.port = port;

    adds[*sizeOfAdds]=newAdvertising;
    (*sizeOfAdds)++;
}