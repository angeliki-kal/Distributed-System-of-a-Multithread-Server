#include "ReadArgs.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int THREADS;
int SERV_PORT;
char* QUERY_FILE;
std::string SERV_IP;

void ReadArgs(int argc, char** argv){
  int arg_index=1;
  int q_flag=0, w_flag=0, sp_flag=0, sip_flag=0;

  //scan all args to find identifiers
  while(arg_index < argc){
      //make sure there is an argument to check
      if(argv[arg_index] == NULL)
      break;

      if(strcmp(argv[arg_index], "-q")==0 && q_flag==0 && argv[arg_index+1]!=NULL){
          QUERY_FILE = argv[++arg_index];
          q_flag = 1;
          arg_index++;
          continue;
      }

      if(strcmp(argv[arg_index], "-w")==0 && w_flag==0 && argv[arg_index+1]!=NULL){
          THREADS = atoi(argv[++arg_index]);
          w_flag = 1;
          arg_index++;
          continue;
      }

      if(strcmp(argv[arg_index],"-sp")==0 && sp_flag==0 && argv[arg_index+1]!=NULL){
          SERV_PORT = atoi(argv[++arg_index]);
          sp_flag = 1;
          arg_index++;
          continue;
      }

      if(strcmp(argv[arg_index],"-sip")==0 && sip_flag==0 && argv[arg_index+1]!=NULL){
          SERV_IP = argv[++arg_index];
          sip_flag = 1;
          arg_index++;
          continue;
      }
      cerr << "Invalid command line arguments at: " << argv[arg_index] << endl;
      exit(-2);
  }

  if(!q_flag || !sp_flag || !w_flag || !sip_flag){
      cerr << "Missing command line arguments" << endl;
      exit(-3);
  }else{
      cout << "queryFile " << QUERY_FILE << " numThreads " << THREADS
           << " servPort " << SERV_PORT << " servIP " << SERV_IP << endl;
  }
}