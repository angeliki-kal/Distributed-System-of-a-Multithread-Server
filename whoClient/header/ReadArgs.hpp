#ifndef READARGS_HPP
#define READARGS_HPP

#include <string>

extern int THREADS;
extern int SERV_PORT;
extern char* QUERY_FILE;
extern std::string SERV_IP;

void ReadArgs(int argc, char** argv);

#endif