#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <pthread.h>
#include "ClientThread.hpp"

#include "ReadArgs.hpp"

using namespace std;


// run with: ./whoClient -q file -w 2 -sp 33333 -sip localhost

int main(int argc, char** argv){
    ReadArgs(argc, argv);
    
    pthread_t* Threads = (pthread_t*)malloc(THREADS*sizeof(pthread_t));

    // open file
    ifstream file(QUERY_FILE);
    if(!file.is_open()){
        cerr << "Can't open " << QUERY_FILE <<endl;
        exit(-1);
    }

    //read file line by line
    string lines[100];
    int index = 0;
    while(getline(file, lines[index]) && index<THREADS){
        if(file.eof()){
            break;
        }

        //create thread for this request
        printf("%s\n", lines[index].c_str());
        pthread_create(&Threads[index], 0, ClientThread, (void*)lines[index].c_str());
        index++;
    }

    // wait for all threads to finish before master exiting
    for(int i=0; i<THREADS; i++){
        pthread_join(Threads[i],NULL);
    }
    free(Threads);
    file.close();
    return 0;
}