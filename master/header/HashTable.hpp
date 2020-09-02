#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "RecordsHashTable.hpp"
#include "BSTree.hpp"

class RecordData {
  public:
  std::string record_key; //i.e country or disease
  BSTree *bst;

  RecordData(std::string key, PatientRecord* pat_rec):record_key(key) {
    bst = new BSTree(pat_rec);
  };
  
  ~RecordData(){
    delete bst;
  };

  void Insert(PatientRecord* pat_rec){
    bst = bst->insert(pat_rec);
  };

  void numOfPatients(std::string date1, std::string date2) {
    cout << record_key << " " << bst->numOfChildren(date1,date2) << endl;
  };

  int numOfPatientsStillSick() {
    cout << record_key << " " << bst->numOfPatientsStillSick() << endl;
  }
};

class Bucket {
    int max_num_records, num_records;
    Bucket *nextBucket; //next bucket of the chain
    RecordData** Records;    //this buckets records
  public:
    //Constructor
    Bucket(int b_size)
    :max_num_records(b_size/sizeof(RecordData*)),num_records(0),nextBucket(NULL){
      Records = (RecordData**)malloc(max_num_records*sizeof(RecordData*));
    };

    //Destructor
    ~Bucket(){
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
    //returns 1 if new record was created, 0 if it already exists
    int Insert(std::string rec_key, PatientRecord* pat_rec){
      //find if RecordData is already in bucket
      RecordData* rec_data_found = NULL;
      for(int i=0; i<num_records; i++){
        if(rec_key.compare(Records[i]->record_key)==0){
          rec_data_found = Records[i];
        }
      }

      if(rec_data_found == NULL) {
        //not found in current bucket
        if(num_records < max_num_records){
          //create and insert to this bucket
          Records[num_records++] = new RecordData(rec_key, pat_rec);
          return 1; //not found had to create new
        }
        else{
          //look if its in the next bucket
          if(nextBucket == NULL)  //bucket overflow, create new bucket
            nextBucket = new Bucket(max_num_records*sizeof(RecordData*));
          return nextBucket->Insert(rec_key,pat_rec);
        }
      }
      else {
        //found in current bucket, insert PatientRecord in the RecordData
        rec_data_found->Insert(pat_rec);
        return 0;
      }
      
    };

    //Find and return record in bucket chain. NULL if not found
    RecordData* Find(std::string recID){
      for(int i=0; i<num_records; i++){
        if(recID.compare(Records[i]->record_key)==0){
          return Records[i];
        }
      }
      //not in this bucket, search next
      if(nextBucket == NULL) {
        return NULL;
      }
      else {
        return nextBucket->Find(recID);
      }
    };

    //Print buckets contents
    void print(){
      std::cout << "\t\t" << num_records << " records" << std::endl;
      for(int i=0; i<max_num_records; i++){
        if(i>=num_records){
          std::cout << "\t\t" << i <<  ")---" << std::endl;
        }
        else{
          std::cout << "\t\t" << i << ")";
          std::cout << Records[i]->record_key << std::endl;
        }
      }
      if(nextBucket != NULL){
        std::cout << "\tOverflow Bucket with:" << std::endl;
        nextBucket->print();
      }
    };

    void printAll_NumOfPatients(std::string date1, std::string date2) {
      for(int i=0; i<num_records; i++) {
        Records[i]->numOfPatients(date1, date2);
        //Records[i]->bst->print2DUtil(0);
      }
      if(nextBucket != NULL) {
        nextBucket->printAll_NumOfPatients(date1, date2);
      }
    };

    void printAll_numOfPatientsStillSick() {
      for(int i=0; i<num_records; i++) {
        Records[i]->numOfPatientsStillSick();
        //Records[i]->bst->print2DUtil(0);
      }
      if(nextBucket != NULL) {
        nextBucket->printAll_numOfPatientsStillSick();
      }
    };
};




class HashTable {
    int table_size;   //total num of buckets in hashtable
    Bucket **Buckets;  //all the buckets of the hashtable  
  public:
    //Constructor
    HashTable(int t_size, int bucket_size)
    :table_size(t_size)
    {
      //init buckets
      Buckets = (Bucket**)malloc(table_size*sizeof(Bucket*));
      for(int i=0; i<table_size; i++){
        Buckets[i] = new Bucket(bucket_size);
      }
    };

    //Destructor
    ~HashTable(){
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

    RecordData* Find(std::string recordID){
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

    Bucket* getBucket(int i) {
      return Buckets[i];
    };

    void printAll_NumOfPatients(std::string date1, std::string date2) {
        for(int i=0; i<table_size; i++) {
          Buckets[i]->printAll_NumOfPatients(date1, date2);
        }
    };

    void print_numOfPatientsStillSick() {
      for(int i=0; i<table_size; i++) {
          Buckets[i]->printAll_numOfPatientsStillSick();
        }
    };


};



#endif
