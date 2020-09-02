#ifndef READFILE_HPP
#define READFILE_HPP

#include <fstream>
#include "PatientRecord.hpp"
#include "HashTable.hpp"

extern char* patientRecordsFile;
extern int h1, h2, bucketSize;

void ReadCmdArgs(int argc, char** argv);

//void ReadPatientRecordsFile(char* filename, HashTable<Records> &RecordContainer);
void ReadPatientRecordFile(char* filename, RecordsHashTable &, HashTable &d, HashTable &c);

/*Reverse date format from dd-mm-yyyy to yyyy-mm-dd*/
std::string yyyy_mm_dd(std::string &datestr);

#endif
