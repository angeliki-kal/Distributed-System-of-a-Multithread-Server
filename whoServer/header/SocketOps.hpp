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


int create_socket(int port){
    int sock;
    if( (sock=socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket:");
        exit(3);
    }
    int temp=1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    struct sockaddr_in server;
    struct sockaddr* serverptr = (struct sockaddr*)&server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY); //bind to all interfaces
    server.sin_port = htons(port);
    if( bind(sock, serverptr, sizeof(server)) < 0){
        perror("bind: ");
        exit(4);
    }

    if( listen(sock,256) < 0){
        perror("listen:");
        exit(5);
    }
    return sock;
}


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
    if(connect(sock,serverptr,sizeof(server)) < 0){
        perror("server connect(): ");
        exit(6);
    }
    return sock;
}

void listen_for_commands(int command_socket){
    printf("staring command socket\n");
    int sock;
    if( (sock=accept(command_socket,NULL,NULL)) < 0){
        perror("accept on command socket ");
        return;
    }
    //got connection, listen for commands
    char* command = (char*)malloc(sizeof(char)*10);
    do{
      int rsize = read(sock,command,10);
      command[rsize-2] = '\0';
      if(strcmp(command,"STATS") == 0){
          printf("STATS\n");
          char* buffer = (char*)malloc(256);
        //   sprintf(buffer, "NumberOfBytes: %lu\n"
        //                   "NumberOfPages: %lu\n"
        //                   "UpTime: %lusec\n",
        //                    NumberOfBytes, NumberOfPages, 
        //                    time(NULL)-StartTime);
          write(sock,buffer,strlen(buffer));
          free(buffer);
          continue;
      }
      else if(strcmp(command,"SHUTDOWN") == 0 || rsize<=0){
          printf("Shutting down server.\n");
          break;
      }
      else{
          if(command != NULL)
              printf("Server received unknown command: %s\n", command);
          else
              printf("Server received NULL command.\n");
          continue;
      }
    }while(1);
    close(sock);
    free(command);
    return;
  }

#endif