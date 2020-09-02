#ifndef RECORDSHASHTABLE_HPP
#define RECORDSHASHTABLE_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "PatientRecord.hpp"

class RecordsBucket {
    int max_num_records, num_records;
    RecordsBucket *nextBucket; //next bucket of the chain
    PatientRecord** Records;    //this buckets records
  public:
    //Constructor
    RecordsBucket(int b_size)
    :max_num_records(b_size/sizeof(PatientRecord*)),num_records(0),nextBucket(NULL){
      Records = (PatientRecord**)malloc(max_num_records*sizeof(PatientRecord*));
    };

    //Destructor
    ~RecordsBucket(){
      //delete next bucket
      if(nextBucket != NULL)
        delete nextBucket;
      //delete records
      for(int i=0; i<num_records; i++){
        delete Records[i];
      }
      //free Record array
      free(Records);
    };

    //Insert record to bucket
    void Insert(std::string recID, PatientRecord* rec){
      if(num_records < max_num_records){
        Records[num_records++] = rec;
      }
      else{
        //overflow, create next Bucket
        if(nextBucket == NULL)
          nextBucket = new RecordsBucket(max_num_records*sizeof(PatientRecord*));
        nextBucket->Insert(recID,rec);
      }
    };

    //Find and return record in bucket chain. NULL if not found
    PatientRecord* Find(std::string recID){
      for(int i=0; i<num_records; i++){
        if(recID.compare(Records[i]->getRecordID()) == 0){
          return Records[i];
        }
      }
      //not in this bucket, search next
      if(nextBucket == NULL)
        return NULL;
      else
        return nextBucket->Find(recID);
    };

    //Print buckets contents
    void print(){
      std::cout << "\t\t" << num_records << " records" << std::endl;
      for(int i=0; i<max_num_records; i++){
        if(i>=num_records){
          std::cout << "\t\t" << i <<  ")---" << std::endl;
        }
        else{
          std::cout << "\t\t" << i << ")" << Records[i]->getRecordID() << std::endl;
        }
      }
      if(nextBucket != NULL){
        std::cout << "\tOverflow Bucket with:" << std::endl;
        nextBucket->print();
      }
    };
};




class RecordsHashTable {
    int table_size;   //total num of buckets in hashtable
    RecordsBucket **Buckets;  //all the buckets of the hashtable
  public:
    //Constructor
    RecordsHashTable(int t_size, int bucket_size)
    :table_size(t_size)
    {
      //init buckets
      Buckets = (RecordsBucket**)malloc(table_size*sizeof(RecordsBucket*));
      for(int i=0; i<table_size; i++){
        Buckets[i] = new RecordsBucket(bucket_size);
      }
    };

    //Destructor
    ~RecordsHashTable(){
      for(int i=0; i<table_size; i++)
        delete Buckets[i];
      free(Buckets);
    };

    //https://en.wikipedia.org/wiki/Jenkins_hash_function
    int HashFunction(const char* recordID){
      size_t length = strlen(recordID);
      size_t i = 0;
      int hash = 0;
      while (i != length) {
        hash += recordID[i++];  //add every char of recordID
        hash += hash << 10;
        hash ^= hash >> 6;
      }
      hash += hash << 3;
      hash ^= hash >> 11;
      hash += hash << 15;
      return hash%table_size;
    };

    PatientRecord* Find(std::string recordID){
      return Buckets[HashFunction(recordID.c_str())]->Find(recordID);
    };

    //Insert new record
    int Insert(std::string recID, PatientRecord* rec){
      Buckets[HashFunction(recID.c_str())]->Insert(recID,rec);
      return 0; //success
    };

    //Print conents of each bucket
    void print(){
      std::cout << "Hashtable with " << table_size << " buckets." << std::endl;
      for(int i=0; i<table_size; i++){
        std::cout << "\tBucket" << i << " with:" << std::endl;
        Buckets[i]->print();
      }
    };

    RecordsBucket* getBucket(int i) {
      return Buckets[i];
    }
};

#endif
