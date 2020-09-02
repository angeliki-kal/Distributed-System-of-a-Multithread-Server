#ifndef PIPES_OPS_HPP
#define PIPES_OPS_HPP

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

extern int BUFFER_SIZE;


void make_twin_pipes(int worker_pid){
    std::stringstream ss;
    ss << worker_pid;
    string pipename_to = "to"+ss.str();
    string pipename_from = "from"+ss.str();
    
    if( mkfifo(pipename_to.c_str(),0666) == -1){
        exit(-1);
    }

    if( mkfifo(pipename_from.c_str(),0666) == -1){
        exit(-1);
    }
    cout << "made one pipe pair" << endl << flush;
}


/*
Open one pipe for writing to worker and one for reading from worker.
Pipe name based on worker pid.
Pipe permission flags based on is_parent parameter. if 0 then is_worker else parent flags are set
*/
void open_pipe(int worker_pid, int* to_fd, int* from_fd, int is_parent) {
    std::stringstream ss;
    ss << worker_pid;
    string pipe_to = "to"+ss.str();
    string pipe_from = "from"+ss.str();

    // cout << getpid() << ": opening pipes" << endl <<  flush;

    int to_flag = O_RDONLY, from_flag = O_WRONLY;
    if(is_parent) {
        to_flag = O_WRONLY;
        from_flag = O_RDONLY;
    }

    if((*to_fd = open(pipe_to.c_str(), to_flag)) < 0) {
        exit(-1);
    }

    if((*from_fd = open(pipe_from.c_str(), from_flag)) < 0) {
        exit(-1);
    }
}


void delete_pipe(int worker_pid, int fd_to, int fd_from) {
    std::stringstream ss;
    ss << worker_pid;
    string pipe_to = "to"+ss.str();
    string pipe_from = "from"+ss.str();

    close(fd_to);
    close(fd_from);
    unlink(pipe_to.c_str());
    unlink(pipe_from.c_str());
}


void write_pipe(int fd, const char* msg) {
    cout << getpid() << ": writing " << msg << endl << flush;
    unsigned int bytes_sent = 0;
    while(bytes_sent < strlen(msg)+1) {
        write(fd, msg, BUFFER_SIZE);
        msg += BUFFER_SIZE;
    }
    //write END OF TRANSMISSION
    write(fd,"EOT",BUFFER_SIZE);
}

char* read_pipe(int fd) {
    cout << getpid() << ": reading" << endl << flush;
    char* msg = NULL;
    unsigned int msg_size = 0;
    char* buffer = (char*)malloc(BUFFER_SIZE);
    do {
        read(fd, buffer, BUFFER_SIZE);
        if(strcmp(buffer, "EOT")) {
            return msg;
        }
        // save buffer to msg
        msg_size += BUFFER_SIZE;
        msg = (char*)realloc(msg, msg_size);
        int offset = msg_size - BUFFER_SIZE;
        memcpy(msg+offset, buffer, BUFFER_SIZE);
    }while(1);
}


#endif