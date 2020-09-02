#ifndef SERVERTHREAD_HPP
#define SERVERTHREAD_HPP

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
#include <sys/select.h>
#include <string>

#include "SocketOps.hpp"

char* read_sock(int sock);
int forward_request(char* request);
int get_worker_pid(char* message);
void set_worker_port(char* message, int worker_pid);
int get_worker_port(int worker_pid);
void set_worker_sock(int worker_pid, int socket);
int get_worker_sock(int worker_pid);
int select_worker(char* message);


void* ServerThread(void* sockets_arg) {
    int* sockets = (int*)sockets_arg;
    int server_sock = sockets[0], query_sock = sockets[1];

    printf("server thread%u start %d %d\n", (unsigned int)pthread_self(), sockets[0], query_sock);

    // init poll socket set
    int num_sockets = 2;
    struct pollfd PollSockets[num_sockets];
    for(int i=0; i<num_sockets; i++) {
        PollSockets[i].fd = sockets[i];
        PollSockets[i].events = POLLIN;
    }

    while(1) {
        // block until connect from worker arrives about stats
        // or query arrives from whoclient

        // fd_set readfds;
        // FD_ZERO(&readfds);  // init fd set
        // FD_SET(3, &readfds);
        // FD_SET(4, &readfds);
        // select(4+1, &readfds, NULL, NULL, NULL);
        
        printf("\nthread%u polling...\n", (unsigned int)pthread_self()); 
        int val = poll(PollSockets, (nfds_t)num_sockets, -1);

        if(PollSockets[0].revents == POLLIN) {  // request from worker
            int worker_session_sock;
            if( (worker_session_sock=accept(server_sock, NULL, NULL)) < 0){
                perror("server_thread accept: ");
                return NULL;
            }
            printf("thread%u accepted worker request (at fd %d)\n", (unsigned int)pthread_self(), sockets[0]);
            // read open connection
            char* message = read_sock(worker_session_sock);
            if(message[0] == '#') { // set new worker port
                int worker_pid = get_worker_pid(message);
                set_worker_port(message, worker_pid);
                printf("Server got worker%d port %d\n", worker_pid, get_worker_port(worker_pid));
                // open connection to worker
                // int worker_socket = connect_server("localhost", get_worker_port(worker_pid));
                // set_worker_sock(worker_pid, worker_socket);
                // // // add worker_socket to poll stockets
                // PollSockets[2].fd = worker_socket;
                // PollSockets[2].events = POLLIN; 
            }
            else {
                printf("Server got stats from worker: %s\n", message);
                printf("thread%u read: %s\n", (unsigned int)pthread_self(), message);
            }
            
            // close connection
            free(message);
            close(worker_session_sock);
        }

        else if (PollSockets[1].revents == POLLIN) {    // request from client
            int client_session_sock;
            if( (client_session_sock=accept(query_sock, NULL, NULL)) < 0){
                perror("server_thread accept: ");
                return NULL;
            }
            printf("thread%u accepted client request (at fd %d)\n", (unsigned int)pthread_self(), sockets[1]);
            // read request from client
            char* message = read_sock(client_session_sock);
            printf("thread%u read request: %s\n", (unsigned int)pthread_self(), message);
            //forward request to worker
            int worker_sock = forward_request(message);
            if( worker_sock != -1) {
                // wait for response from worker TODO: make the server not wait but add worker_sock to pollsockets
                char* answer = read_sock(worker_sock);
                close(worker_sock);
                // answer back to client
                write(client_session_sock, answer, strlen(answer)+1);
                // close connection
                free(answer);
            }
            else {
                const char* answer = "no worker available to answer. Start master and retry.\n";
                write(client_session_sock, answer, strlen(answer)+1);
            }
            free(message);
            close(client_session_sock);                    

        }
        else {
            printf("Server poll interrupt. poll event %d %d  \n", PollSockets[0].revents, PollSockets[1].revents);
        }
    }
    
    printf("server thread%u done\n", (unsigned int)pthread_self());
}


int forward_request(char* request) {
    int worker_pid = select_worker(request);
    if (worker_pid == -1) {
        return -1;
    }
    int worker_port = get_worker_port(worker_pid);
    printf("Server forwarding to worker%d request: %s\n", worker_pid, request);
    int worker_socket = connect_server("localhost", get_worker_port(worker_pid));
    // forward request to worker socket
    write(worker_socket, request, strlen(request)+1);
    return worker_socket;
}

char* read_sock(int sock){
    int buffer = 100;
    char* message = (char*)malloc(buffer*sizeof(char));
    int offset=0;

    //read message chunk by chunk
    while(read(sock, message+offset, buffer) == buffer){
      offset += buffer;
      message = (char*)realloc(message,offset+buffer);
    }
    return message;
}

#include <map>

std::map<int,int> port_map; 
std::map<int,int> sock_map;

int select_worker(char* message) {
    if (port_map.size() == 0) {
        printf("NO WORKER SOCKET SET. CANNOT ANSWER. Start the master and retry\n");
        return -1;
    }
    std::map<int,int>::iterator item = port_map.begin();
    int random = strlen(message) % port_map.size();
    std::advance(item, random);
    return item->first;
}

int get_worker_pid(char* message) {
    std::string str = std::string(message);
    unsigned first = str.find('#');
    unsigned last = str.find('$');
    return atoi(str.substr(first+1,last-first).c_str());
}


void set_worker_port(char* message, int worker_pid) {
    std::string str = std::string(message);
    unsigned first = str.find('$');
    int port = atoi(str.substr(first+1).c_str());
    port_map[worker_pid] = port;
}

int get_worker_port(int worker_pid) {
    return port_map[worker_pid];
}

void set_worker_sock(int worker_pid, int socket) {
    sock_map[worker_pid] = socket;
}

int get_worker_sock(int worker_pid) {
    return sock_map[worker_pid];
}

#endif