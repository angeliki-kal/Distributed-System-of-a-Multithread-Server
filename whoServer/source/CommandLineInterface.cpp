#include "CommandLineInterface.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
//#include <string>
#include <iostream>
#include <sstream>

using namespace std;

void CommandLineInterface(){
    cout << "CommandLine Interface is open." << endl << "$";

    while(1){
        string command;
        std::cin >> command;
        if(command.compare("/diseaseFrequency")==0){
            diseaseFrequency();
            std::cout << "$";
            continue;
        }

        if(command.compare("/topk-AgeRanges")==0){
            topkAgeRanges();
            std::cout << "$";
            continue;
        }    

        if(command.compare("/searchPatientRecord")==0){
            searchPatientRecord();
            std::cout << "$";
            continue;
        }

        if(command.compare("/numPatientAdmissions")==0){
            numPatientAdmissions();
            std::cout << "$";
            continue;
        }

        if(command.compare("/numPatientDischarges")==0){
            numPatientDischarges();
            std::cout << "$";
            continue;
        }

        //didn't match any of the given commands
        cout << "Invalid command: " << command << endl << "$";
    }
}

void diseaseFrequency(){
    std::string virusName, country, date1, date2;
    cin >> virusName;
    cin >> date1;
    cin >> date2;

    if(cin.peek() == '\n') {
        //cout << date1 << date2 <<endl;
        //vir_data->bst->print2DUtil(0);
        cout << virusName <<   endl; 
    }
    else {
        cin >> country;
        //country_data->bst->print2DUtil(0);
        cout << virusName << endl;  
    }
    return;
}

void topkAgeRanges(){
    std::string country, disease, date1, date2;
    int k;
    cin >> k;
    cin >> country;
    cin >> disease;
    cin >> date1;
    cin >> date2;
    return;
}

void searchPatientRecord(){
    std::string recordID;
    cin >> recordID;
    return;
}

void numPatientAdmissions(){
    std::string disease, date1, date2;
    cin >> disease;
    cin >> date1;
    cin >> date2;
    if(cin.peek() == '\n'){
        return;
    }
    else(
        cin >> country;
    )
    return;
}

void numPatientDischarges(){
    std::string disease, date1, date2;
    cin >> disease;
    cin >> date1;
    cin >> date2;
    if(cin.peek() == '\n'){
        return;
    }
    else(
        cin >> country;
    )
    return;
}

