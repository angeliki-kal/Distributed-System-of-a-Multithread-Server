#ifndef PATIENTRECORD_HPP
#define PATIENTRECORD_HPP

#include <string>
#include <iostream>

class PatientRecord {
    std::string recordID;
    std::string patientFirstName;
    std::string patientLastName;
    std::string diseaseID;
    std::string country;
    std::string entryDate;
    std::string exitDate;
  public:
    PatientRecord(std::string id, std::string pfName, std::string plName, std::string diseaseId,
    std::string country, std::string entryDate, std::string exitDate);
    ~PatientRecord();
    //void print();

    //getters setters
    std::string getRecordID() {
      return this->recordID;
    }
    void setRecordID(std::string recordID) {
      this->recordID = recordID;
    }

    std::string getPatientFirstName() {
      return this->patientFirstName;
    }
    void setPatientFirstName(std::string patientFirstName) {
      this->patientFirstName = patientFirstName;
    }

    std::string getPatientLastName() {
      return this->patientLastName;
    }
    void setPatientLastName(std::string patientLastName) {
      this->patientLastName = patientLastName;
    }

    std::string getDiseaseID() {
      return this->diseaseID;
    }
    void setDiseaseID(std::string diseaseID) {
      this->diseaseID = diseaseID;
    }

    std::string getCountry() {
      return this->country;
    }
    void setCountry(std::string country) {
      this->country = country;
    }

    std::string getEntryDate() {
      return this->entryDate;
    }
    void setEntryDate(std::string entryDate) {
      this->entryDate = entryDate;
    }

    std::string getExitDate() {
      return this->exitDate;
    }
    void setExitDate(std::string exitDate) {
      this->exitDate = exitDate;
    }
};

#endif
