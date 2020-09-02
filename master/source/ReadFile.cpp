#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <fstream>

#include "ReadFile.hpp"
#include "HashTable.hpp"

using namespace std;

char* patientRecordsFile;
int h1, h2, bucketSize;

void ReadCmdArgs(int argc, char** argv) {
  int arg_index=1;
  int p_flag=0, h1_flag=0, h2_flag=0, b_flag=0;

  //scan all arguements to find identifiers
  while(arg_index < argc){
    //make sure there is an argument to check
    if(argv[arg_index] == NULL)
      break;

    if(strcmp(argv[arg_index],"-p")==0 && p_flag==0 && argv[arg_index+1]!=NULL){
      patientRecordsFile = argv[++arg_index];
      p_flag = 1;
      arg_index++;
      continue;
    }

    if(strcmp(argv[arg_index],"-h1")==0 && h1_flag==0 && argv[arg_index+1]!=NULL){
      h1 = atoi(argv[++arg_index]);
      h1_flag = 1;
      arg_index++;
      continue;
    }
    if(strcmp(argv[arg_index],"-h2")==0 && h2_flag==0 && argv[arg_index+1]!=NULL){
      h2 = atoi(argv[++arg_index]);
      h2_flag = 1;
      arg_index++;
      continue;
    }
    if(strcmp(argv[arg_index],"-b")==0 && b_flag==0 && argv[arg_index+1]!=NULL){
      bucketSize = atoi(argv[++arg_index]);
      b_flag = 1;
      arg_index++;
      continue;
    }
    cerr << "Invalid command line arguments at: " << argv[arg_index] << endl;
    exit(-2);
  }

  if(!p_flag  && !h1_flag && !h2_flag && !b_flag){
    cerr << "Missing command line arguments" << endl;
    exit(-3);
  }else{
    cout << " patientRecordsFile " << patientRecordsFile << " diseaseHashtableNumOfEntries "
         << " countryHashtableNumOfEntries " << " bucketSize " << bucketSize << endl;
  }
}

void ReadPatientRecordFile(char* filename, RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht) {
  //open file
  ifstream file(filename);
  if(!file.is_open()){
    cerr << "Can't open " << filename << endl;
    exit(-1);
  }

  //read 7 strings
  while (1){
    std::string recordID,patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate;
    file >> recordID;
    if(file.eof()) {
      break;
    }
    file >> patientFirstName;
    file >> patientLastName;
    file >> diseaseID;
    file >> country;
    file >> entryDate;
    file >> exitDate;
    entryDate = yyyy_mm_dd(entryDate);
    if(exitDate.compare("-") != 0)
      exitDate = yyyy_mm_dd(exitDate);
    //check for duplicate records
    PatientRecord *pat_rec_found = NULL;
    if( (pat_rec_found = records_ht.Find(recordID)) != NULL) {
      cout << "----" << pat_rec_found << "----" << endl;
       cout << "FOUND RECORD THAT IS NOT UNIQUE " << recordID << pat_rec_found->getRecordID()  << endl;
       exit(-1);
    }
    //check record dates
    if(entryDate.compare(exitDate) > 0 && exitDate.compare("-") != 0) {
      cout << "EXIT DATE IS BEFORE END DATE | entry:" << entryDate << ", exit:" << exitDate << endl;
      continue;
    }
    //insert to hashtables
    PatientRecord *rec = new PatientRecord(recordID, patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
    records_ht.Insert(rec->getRecordID(), rec);
    disease_ht.Insert(rec->getDiseaseID(), rec);
    country_ht.Insert(rec->getCountry(), rec);
   }
   file.close();
}

/*Reverse date format from dd-mm-yyyy to yyyy-mm-dd*/
std::string yyyy_mm_dd(std::string &datestr){
  return datestr.substr(6,4)+datestr.substr(3,2)+datestr.substr(0,2);
}
