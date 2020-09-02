#ifndef CLIENT_THREAD_HPP
#define CLIENT_THREAD_HPP

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <string>

#include "SocketOps.hpp"
#include "ReadArgs.hpp"

using namespace std;


void* ClientThread(void* request) {
    // connect to server
    int server_sock = connect_server(SERV_IP.c_str(), SERV_PORT);
    printf("thread%d connected to server\n", (unsigned int)pthread_self());
    // send request to server
    write(server_sock, request, strlen((char*)request)+1);
    printf("thread%d sent request %s\n", (unsigned int)pthread_self(), (char*)request);
    // wait for answer from server, when arrives read it
    char* answer = read_sock(server_sock);
    // print answer
    printf("thread%u got answer: %s\n", (unsigned int)pthread_self(), answer);
    free(answer); 
    return NULL;
}

#endif