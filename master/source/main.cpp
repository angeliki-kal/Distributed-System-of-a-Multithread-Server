#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>

#include "Worker.hpp"
#include "PipeOperations.hpp"

using namespace std;


int NUM_WORKERS, BUFFER_SIZE, SERVER_PORT;
char* INPUT_DIR;
string SERVER_IP;

void read_args(int argc, char** argv);


// run with: ./master -w 2 -b 100 -s localhost -p 44444 -i ./bla

int main(int argc, char** argv){
    read_args(argc, argv);

    // create children
    int chld_pids[NUM_WORKERS];
    for(int i=0; i<NUM_WORKERS; i++) {
        chld_pids[i] = fork();
        if( chld_pids[i] == 0) {
            // child
            return worker_func(SERVER_IP, SERVER_PORT);
        }
    }

    // create_pipes
    int to_pipes_fds[NUM_WORKERS];
    int from_pipes_fds[NUM_WORKERS];
    for(int i=0; i<NUM_WORKERS; i++) {
        make_twin_pipes(chld_pids[i]);
    }
    // open pipes
    for(int i=0; i<NUM_WORKERS; i++) {
        open_pipe(chld_pids[i], &to_pipes_fds[i], &from_pipes_fds[i], 1);
    }


    // assign directories to children
    for(int i=0; i<NUM_WORKERS; i++) {
        char* str = read_pipe(from_pipes_fds[i]);
        printf("parent read %d\n", chld_pids[i]);
    }

    cout << "main exit" << endl << flush;
    // close and delete pipes
    for(int i=0; i<NUM_WORKERS; i++) {
        delete_pipe(chld_pids[i], to_pipes_fds[i], from_pipes_fds[i]);
    }

    return 0;
}


void read_args(int argc, char** argv) {
    int arg_index=1;
    int w_flag=0, i_flag=0, b_flag=0, s_flag=0, p_flag=0;

    //scan all arguements to find identifiers
    while(arg_index < argc){
        //make sure there is an argument to check
        if(argv[arg_index] == NULL)
            break;

        if(strcmp(argv[arg_index],"-w")==0 && w_flag==0 && argv[arg_index+1]!=NULL){
            NUM_WORKERS = atoi(argv[++arg_index]);
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

        if(strcmp(argv[arg_index],"-s")==0 && s_flag==0 && argv[arg_index+1]!=NULL){
            SERVER_IP = argv[++arg_index];
            s_flag = 1;
            arg_index++;
            continue;
        }   

        if(strcmp(argv[arg_index],"-p")==0 && p_flag==0 && argv[arg_index+1]!=NULL){
            SERVER_PORT = atoi(argv[++arg_index]);
            p_flag = 1;
            arg_index++;
            continue;
        } 

        if(strcmp(argv[arg_index],"-i")==0 && i_flag==0 && argv[arg_index+1]!=NULL){
            INPUT_DIR = argv[++arg_index];
            i_flag = 1;
            arg_index++;
            continue;
        }
        cerr << "Invalid command line arguments at: " << argv[arg_index] << endl;
        exit(-2);
      }

      if(!w_flag  || !i_flag || !b_flag || !b_flag){
          cerr << "Missing command line arguments" << endl;
          exit(-3);
      }else{
          cout << " num_workers " << NUM_WORKERS << " buffer_size "
               << BUFFER_SIZE << " input_dir " << INPUT_DIR << endl;
    }
}
