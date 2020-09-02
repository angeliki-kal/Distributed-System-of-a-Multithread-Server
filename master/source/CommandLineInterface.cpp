#include "CommandLineInterface.hpp"
#include "ReadFile.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
//#include <string>
#include <iostream>
#include <sstream>

using namespace std;

void CommandLineInterface(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht) {
  std::cout << "CommandLine Interface is open." << endl << "$";

  while(1) {
    string command;
    cin >> command;
    if(command.compare("/globalDiseaseStats") == 0){
      globalDiseaseStats(disease_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/diseaseFrequency") == 0) {
      diseaseFrequency(disease_ht,country_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/topk-Diseases") == 0) {
      topkDiseases(country_ht, disease_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/topk-Countries") == 0) {
      topkCountries(country_ht, disease_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/recordPatientExit") == 0) {
      recordPatientExit(records_ht, disease_ht, country_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/insertPatientRecord") == 0) {
      insertPatientRecord(records_ht, disease_ht, country_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/diseaseFrequency") == 0) {
      diseaseFrequency(disease_ht,country_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/numCurrentPatients") == 0) {
      numCurrentPatients(disease_ht);
      std::cout << "$";
      continue;
    }

    if(command.compare("/exit") == 0) {
      std::cout << "exiting\n";
      break;
    }

    //didn't match any command
    cout << "Invalid command: " << command << std::endl;
    cout << "$";
  }
}

void globalDiseaseStats(HashTable &disease_ht) {
  string date1, date2;

  if(cin.peek() == '\n') { //no date set
    disease_ht.printAll_NumOfPatients(date1, date2);
    std::cout << endl;
    return;
  }
  else {
    cin >> date1;
    if(cin.peek() == '\n') {
      cout << "Missing date2" << endl;
      return;
    }
    else {
      cin >> date2;
      date1 = yyyy_mm_dd(date1);
      date2 = yyyy_mm_dd(date2);
      if(date1.compare(date2) > 0) {
        cout << "date2 is before entry date1 | date1:" << date1 << ", date2:" << date2 << endl;
        return;
      }
      else {
        disease_ht.printAll_NumOfPatients(date1,date2);
        cout << endl;
      }
    }
  }
}

void diseaseFrequency(HashTable &disease_ht, HashTable &country_ht) {
  std::string virusName, country, date1, date2;
  cin >> virusName;
  cin >> date1;
  cin >> date2;
  date1 = yyyy_mm_dd(date1);
  date2 = yyyy_mm_dd(date2);
  RecordData* vir_data = disease_ht.Find(virusName);
  if(vir_data == NULL) {
      cout << virusName << " not found in disease_ht" << endl;
      return;
  }

  if(cin.peek() == '\n') {
    //cout << date1 << date2 <<endl;
    //vir_data->bst->print2DUtil(0);
    cout << virusName << " " << vir_data->bst->numOfChildren(date1,date2) << endl;
  }
  else {
    cin >> country;
    RecordData *country_data = country_ht.Find(country);
    if(country_data == NULL){
      cout << "this country doesn't exist" << endl;
      return;
    }
    //country_data->bst->print2DUtil(0);
    cout << virusName << " " << country_data->bst->numOfChildren(date1,date2,virusName) << endl;
  }
}

void topkDiseases(HashTable &country_ht, HashTable &disease_ht) {
  std::string country, date1, date2;
  int k;
  cin >> k;
  cin >> country;
  if(cin.peek() == '\n'){
    return;
  }
  else{
    cin >> date1;
    cin >> date2;
    date1 = yyyy_mm_dd(date1);
    date2 = yyyy_mm_dd(date2);

    if(date1.compare(date2) > 0){
      cout << "Date1 is afte Date2, bad formatting." << endl;
      return;
    }
  }
  return;
}

void topkCountries(HashTable &country_ht, HashTable &disease_ht) {
  std::string diseaseID, date1, date2;
  int k;
  cin >> k;
  cin >> diseaseID;
  if(cin.peek() == '\n'){
    return;
  }
  else{
    cin >> date1;
    cin >> date2;
    date1 = yyyy_mm_dd(date1);
    date2 = yyyy_mm_dd(date2);

    if(date1.compare(date2) > 0){
      cout << "Date1 is afte Date2, bad formatting." << endl;
      return;
    }

  }
  return;
}

void insertPatientRecord(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht){
std::string recordID,patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate;
    //char last_char_read = 'a'; //just dont be EOF
    cin >> recordID;
    cin >> patientFirstName;
    cin >> patientLastName;
    cin >> diseaseID;
    cin >> country;
    cin >> entryDate;
    if(cin.peek() == '\n') {
      if (recordID.empty() || patientFirstName.empty() || patientLastName.empty() || diseaseID.empty() ||
        country.empty() || entryDate.empty()){
        cout << "Bad formatting for /insertPatientRecord\n";
        return;
      }
      //check if it's a duplicate
      PatientRecord *pat_rec_found = NULL;
      if( (pat_rec_found = records_ht.Find(recordID)) != NULL) {
        cout << "----" << pat_rec_found << "----" << endl;
        cout << "record is not unique " << recordID << " vs " << pat_rec_found->getRecordID()  << endl;
        return;
      }
      //insert to hashtables
      PatientRecord *rec = new PatientRecord(recordID, patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
      records_ht.Insert(rec->getRecordID(), rec);
      disease_ht.Insert(rec->getDiseaseID(), rec);
      country_ht.Insert(rec->getCountry(), rec);
      cout << "Record added" << endl;
    }
    else{
      cin >> exitDate;
      entryDate = yyyy_mm_dd(entryDate);
      if(exitDate.compare("-") != 0)
        exitDate = yyyy_mm_dd(exitDate);

      //check if it's a duplicate
      PatientRecord *pat_rec_found = NULL;
      if( (pat_rec_found = records_ht.Find(recordID)) != NULL) {
        cout << "----" << pat_rec_found << "----" << endl;
        cout << "record is not unique " << recordID << " vs " << pat_rec_found->getRecordID()  << endl;
        return;
      }
      //check record dates
      if(entryDate.compare(exitDate) > 0 && exitDate.compare("-") != 0) {
        cout << "exit date is before entry date | entry:" << entryDate << ", exit:" << exitDate << endl;
        return;
      }
      //insert to hashtables
      PatientRecord *rec = new PatientRecord(recordID, patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
      records_ht.Insert(rec->getRecordID(), rec);
      disease_ht.Insert(rec->getDiseaseID(), rec);
      country_ht.Insert(rec->getCountry(), rec);
      cout << "Record added" << endl;
    }
}

void recordPatientExit(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht){
  std::string recordID, exitDate;
  cin >> recordID;
  cin >> exitDate;
  if (recordID.empty() || exitDate.empty()){
		cout << "Bad formatting for /recordPatientExit" << endl;
		return;
	}
  PatientRecord *rec = records_ht.Find(recordID);
  if(rec == NULL){
    cout << "This recordID " << recordID << " doesn't exist" << endl;
    return;
  };
  exitDate = yyyy_mm_dd(exitDate);
  rec->setExitDate(exitDate);
  cout << "Record updated" << endl;
}

void numCurrentPatients(HashTable &disease_ht){
  std::string diseaseID;
  if(cin.peek() == '\n') { //no date set
    disease_ht.print_numOfPatientsStillSick();
    return;
  }
  else {
    cin >> diseaseID;
    cout << diseaseID << " " << disease_ht.Find(diseaseID)->bst->numOfPatientsStillSick() << endl;
  }

}
