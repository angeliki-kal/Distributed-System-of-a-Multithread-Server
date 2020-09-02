#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#include "SocketOps.hpp"
#include "ServerThread.hpp"

int QUERY_PORT_NUM, STATISTICS_PORT_NUM, THREADS, BUFFER_SIZE;
void ReadArgs(int argc, char** argv);

using namespace std;

// run with: ./whoServer -q 33333 -s 44444 -w 2 -b 200

int main(int argc, char** argv){
    ReadArgs(argc, argv);

    //open socket for receiving stats from workers
    int server_sock = create_socket(STATISTICS_PORT_NUM);
    //open socket for receiving requests from clients
    int query_sock = create_socket(QUERY_PORT_NUM);
    int sockets[] = {server_sock, query_sock};
    printf("socket serv %d\n", sockets[0]);

    //start threads (threads accept() connections on sockets)
    pthread_t* Threads = (pthread_t*)malloc(THREADS*sizeof(pthread_t));
    for(int i=0; i<THREADS; i++){
        pthread_create(&Threads[i],0, ServerThread,  (void*)sockets);
    }

   
    //wait for threads to finish
    for(int i=0; i<THREADS; i++){
        pthread_join(Threads[i],NULL);
    }
    //SHUTDOWN
    close(server_sock);
    free(Threads);
    printf("Server done\n.");
    return 0;
}


void ReadArgs(int argc, char** argv){
    int arg_index=1;
    int q_flag=0, s_flag=0, w_flag=0, b_flag=0;

    //scan all args to find identifiers
    while(arg_index < argc){
        //make sure there is an argument to check
        if(argv[arg_index] == NULL)
        break;

        if( strcmp(argv[arg_index], "-q")==0 && q_flag==0 && argv[arg_index+1]!=NULL){
            QUERY_PORT_NUM = atoi(argv[++arg_index]);
            q_flag = 1;
            arg_index++;
            continue;
        }

        if(strcmp(argv[arg_index], "-s")==0 && s_flag==0 && argv[arg_index+1]!=NULL){
            STATISTICS_PORT_NUM = atoi(argv[++arg_index]);
            s_flag = 1;
            arg_index++;
            continue;
        }

        if(strcmp(argv[arg_index], "-w")==0 && w_flag==0 && argv[arg_index+1]!=NULL){
            THREADS = atoi(argv[++arg_index]);
            w_flag = 1;
            arg_index++;
            continue;
        }

        if(strcmp(argv[arg_index],"-b")==0 && b_flag==0 && argv[arg_index+1]!=NULL){
            BUFFER_SIZE = atoi(argv[++arg_index]);
            b_flag = 1;
            arg_index++;
            continue;
        }
        cerr << "Invalid command line arguments at: " << argv[arg_index] << endl;
        exit(-2);
    }

    if(!q_flag || !s_flag || !w_flag || !b_flag){
        cerr << "Missing command line arguments" << endl;
        exit(-3);
    }else{
        cout << " queryPortNum " << QUERY_PORT_NUM << " statisticsPortNum " << STATISTICS_PORT_NUM
             << " numThreads " << THREADS << " bufferSize " << BUFFER_SIZE << endl;
    }
}