#ifndef BSTREE
#define BSTREE

#include "PatientRecord.hpp"
#include <iostream>
using namespace std;

class BSTree {
        PatientRecord *prec;
        BSTree *left, *right;

        int max(int a, int b) {
            return (a >b) ? a : b;
        };

        BSTree *leftRotate() {
            BSTree *y = right;
            BSTree *T2 = y->left;
            //do rotation
            y->left = this;
            right = T2;
            //update heights
            int left_height = (left != NULL) ? left->height : 0;
            int right_height = (right != NULL) ? right->height : 0;
            height = max(left_height, right_height+1);

            int y_left_height = (y->left != NULL) ? y->left->height : 0;
            int y_right_height = (y->right != NULL) ? y->right->height : 0;
            y->height = max(y_left_height, y_right_height+1);
            //return new root
            return y;
        };

        BSTree *rightRotate() {
            BSTree *x = left;
            BSTree *T2 = x->right;
            //do rotation
            x->right = this;
            left = T2;
            //update heights
            int left_height = (left != NULL) ? left->height : 0;
            int right_height = (right != NULL) ? right->height : 0;
            height = max(left_height, right_height+1);

            int x_left_height = (x->left != NULL) ? x->left->height : 0;
            int x_right_height = (x->right != NULL) ? x->right->height : 0;
            x->height = max(x_left_height, x_right_height+1);
        };

    public:
        int height;
        BSTree(PatientRecord* p):prec(p),left(NULL),right(NULL),height(1){};
        ~BSTree(){
            if(left != NULL) {
                delete left;
            }
            if(right != NULL) {
                delete right;
            }
        };

        BSTree* insert(PatientRecord* new_prec) {
            std::string key = new_prec->getEntryDate();
            //compare left right
            if(key.compare(prec->getEntryDate()) <= 0) { //left only if smaller
                if(left == NULL) {
                    left = new BSTree(new_prec);
                }
                else {
                    left = left->insert(new_prec);
                }
            }
            else {
                if(right == NULL) {
                    right = new BSTree(new_prec);
                }
                else {
                    right = right->insert(new_prec);
                }
            }

            int left_height = (left != NULL) ? left->height : 0;
            int right_height = (right != NULL) ? right->height : 0;
            height = 1 + max(left_height, right_height);  //update height
            int balance = left_height - right_height; //get balance factor
            //unbalanced left left case
            if (left != NULL && balance>1 && (key.compare(left->getKey()) <= 0)) {
                return rightRotate();
            }
            //unbalanced right right case
            if (right != NULL && balance<-1 && (key.compare(right->getKey()) > 0)) {
                return leftRotate();
            }
            //unbalanced left right case
            if (left != NULL && balance>1 && (key.compare(left->getKey()) > 0)) {
                left = left->leftRotate();
                return rightRotate();
            }
            //unbalanced right left case
            if (right != NULL && balance<-1 && (key.compare(right->getKey()) <= 0)) {
                right = right->rightRotate();
                return leftRotate();
            }

            return this;
        };

        std::string getKey() {
            return prec->getEntryDate();
        };

        int numOfChildren(std::string date1, std::string date2) {
            if(date1.empty() && date2.empty()) {
                int left_children = left == NULL ? 0 : left->numOfChildren(date1,date2);
                int right_children = right == NULL ? 0 : right->numOfChildren(date1,date2);
                return 1+left_children+right_children;
            }
            else { //with dates
                bool got_in_before_date2 = prec->getEntryDate().compare(date2) <= 0;
                bool got_in_after_date1 = prec->getEntryDate().compare(date1) >= 0;
                if( got_in_before_date2 && got_in_after_date1) {
                    //cout << prec->getRecordID() << "is in the results" << endl;
                    int left_children = left == NULL ? 0 : left->numOfChildren(date1,date2);
                    int right_children = right == NULL ? 0 : right->numOfChildren(date1,date2);
                    return 1+left_children+right_children;
                }
                else if(!got_in_after_date1) {
                    //in this case this patient record was recorded before date1
                    //so there is no chance that any of the left children got in after date1 either
                    return right == NULL ? 0 : right->numOfChildren(date1,date2);
                }
                else if(!got_in_before_date2) {
                    //in this case this patient record was recorded before date1
                    //so there is no chance that any of the right children got in before date2 either
                    return left == NULL ? 0 : left->numOfChildren(date1,date2);
                }
                //cout << "-----------den prepei na ftasei edw POTE-----------------" << endl;
                exit(-1);
            }
        };

        int numOfChildren(std::string date1, std::string date2, std::string virusName) {
            bool virus_match = virusName.compare(prec->getDiseaseID()) == 0;
            // if(virus_match)
            //     cout << "match for "<< prec->getRecordID() << date1 << " " << date2 << endl;
            //cout << "match:" << virus_match << endl;
            if(date1.empty() && date2.empty()) {
                int left_children = left == NULL ? 0 : left->numOfChildren(date1,date2,virusName);
                int right_children = right == NULL ? 0 : right->numOfChildren(date1,date2,virusName);
                return virus_match ? 1+left_children+right_children : left_children+right_children;
            }
            else { //with dates
                bool got_in_before_date2 = prec->getEntryDate().compare(date2) <= 0;
                bool got_in_after_date1 = prec->getEntryDate().compare(date1) >= 0;
                if( got_in_before_date2 && got_in_after_date1) {
                    //cout << prec->getRecordID() << "is in the results" << endl;
                    int left_children = left == NULL ? 0 : left->numOfChildren(date1,date2,virusName);
                    int right_children = right == NULL ? 0 : right->numOfChildren(date1,date2,virusName);
                    return virus_match ? 1+left_children+right_children : left_children+right_children;
                }
                else if(!got_in_after_date1) {
                    //in this case this patient record was recorded before date1
                    //so there is no chance that any of the left children got in after date1 either
                    return right == NULL ? 0 : right->numOfChildren(date1,date2,virusName);
                }
                else if(!got_in_before_date2) {
                    //in this case this patient record was recorded before date1
                    //so there is no chance that any of the right children got in before date2 either
                    return left == NULL ? 0 : left->numOfChildren(date1,date2,virusName);
                }
                //cout << "-----------den prepei na ftasei edw POTE-----------------" << endl;
                exit(-1);
            }
        };

        int numOfPatientsStillSick() {
            bool still_sick = prec->getExitDate().compare("-") == 0;
            //if(still_sick) cout << prec->getRecordID() << " is still sick with ed:" << prec->getExitDate() << endl;
            int left_children = left == NULL ? 0 : left->numOfPatientsStillSick();
            int right_children = right == NULL ? 0 : right->numOfPatientsStillSick();
            return still_sick ? 1+left_children+right_children : left_children+right_children;
        };

        //used in order to check if some of the commands given by user, are working correctly
        void print2DUtil(int space)  {

            // Increase distance between levels
            space += 4;

            // Process right child first
            if(right != NULL)
                right->print2DUtil(space);

            // Print current node after space
            // count
            cout<<endl;
            for (int i = 4; i < space; i++)
                cout<<" ";
            cout << prec->getRecordID() << "\n";

            // Process left child
            if(left != NULL)
                left->print2DUtil(space);
        };
};

#endif
