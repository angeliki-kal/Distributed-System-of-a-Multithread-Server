#ifndef COMMANDLINEINTERFACE_HPP
#define COMMANDLINEINTERFACE_HPP

#include "PatientRecord.hpp"
#include "HashTable.hpp"
#include "RecordsHashTable.hpp"

void CommandLineInterface(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht);

void globalDiseaseStats(HashTable &disease_ht);

void diseaseFrequency(HashTable &disease_ht, HashTable &country_ht);

void topkDiseases(HashTable &country_ht, HashTable &disease_ht);

void topkCountries(HashTable &country_ht, HashTable &disease_ht);

void insertPatientRecord(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht);

void recordPatientExit(RecordsHashTable &records_ht, HashTable &disease_ht, HashTable &country_ht);

void numCurrentPatients(HashTable &disease_ht);

#endif
