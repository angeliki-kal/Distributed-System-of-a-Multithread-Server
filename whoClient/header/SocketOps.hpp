#ifndef SOCKETOPS_HPP
#define SOCKETOPS_HPP

#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int connect_server(const char* server_ip, int server_port){
    int sock;
    if( (sock=socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket() connect:");
        exit(4);
    }
    //get address
    struct sockaddr_in server;
    struct sockaddr* serverptr=(struct sockaddr*)&server;
    struct hostent* rem;
    struct in_addr host_address;

    //translate host_or_ip
    if(isdigit(server_ip[0])){  //if ip
        inet_aton(server_ip,&host_address);
        rem = gethostbyaddr(&host_address,sizeof(host_address),AF_INET);
    }
    else{   //if host
        if((rem=gethostbyname(server_ip)) == NULL){
            perror("gethostbyname: ");
            exit(5);
        }
    }

    server.sin_family = AF_INET;
    memcpy(&server.sin_addr,rem->h_addr,rem->h_length);
    server.sin_port = htons(server_port);
    //connect to host
    if(connect(sock, serverptr, sizeof(server)) < 0){
        perror("server connect(): ");
        exit(6);
    }
    return sock;
}


char* read_sock(int sock){
    int buffer = 100;
    char* message = (char*)malloc(buffer*sizeof(char));
    int offset=0;
    printf("waiting for read\n");
    //read message chunk by chunk
    while(read(sock, message+offset, buffer) == buffer){
        offset += buffer;
        message = (char*)realloc(message,offset+buffer);
    }
    return message;
}

#endif