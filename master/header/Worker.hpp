#ifndef WORKER_HPP
#define WORKER_HPP

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "PipeOperations.hpp"
#include "SocketOps.hpp"

using namespace std;

extern int LISTEN_PORT;

void send_stats(string& server_ip, int server_port);
void send_port(string& server_ip, int server_port);

int worker_func(string server_ip, int server_port) {
    pid_t mypid = getpid();
    std::stringstream ss;
    ss << "worker answer" << mypid;
    int to_fd, from_fd;
    // cout << mypid << ": worker start" << endl;
    // open_pipe(mypid, &to_fd, &from_fd, 0);
    // write_pipe(from_fd, ss.str().c_str());
    // close(to_fd);
    // close(from_fd);

    /*************** whoServer communication****************/
    //open a socket that listens for requests from server
    int listen_sock = create_socket(0);
    printf("worker%d open for requests on port %d\n", mypid, LISTEN_PORT);

    send_port(server_ip, server_port);
    send_stats(server_ip, server_port);

    // shutdown connection to server
    shutdown(server_port,0);

    // answer requests from server
    listen_for_commands(listen_sock);

    //destroy socket for listening
    close(listen_sock);

    cout << mypid << ": worker exit" << endl; 
    return 0;
}


void send_port(string& server_ip, int server_port) {
    // connect to server
    int server_sock = connect_server(server_ip.c_str(), server_port);
    printf("worker%d connected to server on port %d\n", getpid(), server_port);
    // tell server which port you are listening on for requests (LISTEN_PORT)
    int buf_size = 13;  //1+5+1+5+1 
    char buf[buf_size];
    snprintf(buf, buf_size, "#%d$%d", getpid(), LISTEN_PORT); // "#process_id$port_num"
    write(server_sock, &buf, buf_size);
    close(server_sock);
}


void send_stats(string& server_ip, int server_port) {
    // connect to server
    int server_sock = connect_server(server_ip.c_str(), server_port);
    // send summary statistics to server
    char stats[25];
    snprintf(stats, 25, "worker%d sent stats", getpid());
    printf("%s\n", stats);
    write(server_sock, stats, strlen(stats)+1);
    close(server_sock);
}



#endif