#include "ReadFile.hpp"
#include "PatientRecord.hpp"
#include <cstdio>

using namespace std;

PatientRecord::PatientRecord(std::string id,std::string pfName, std::string plName, std::string diseaseId,
  std::string country, std::string entryDate, std::string exitDate)
:recordID(id), patientFirstName(pfName), patientLastName(plName),
diseaseID(diseaseId), country(country), entryDate(entryDate), exitDate(exitDate)
{
  //print();
}

PatientRecord::~PatientRecord(){
}

// void PatientRecord::print(){
//   cout << "PatientRecord::print " << recordID << endl;
// }
