#include "stratified_resample.h"
#include "stratified_random.h"
#include <iostream>

using namespace std;

void stratified_resample(VectorXf w, vector<int> &keep, float &Neff)
{
    VectorXf wsqrd(w.size());
    for (int i=0; i<w.size(); i++) {
        w(i) = w(i)/w.sum();
        wsqrd(i) = pow(w(i),2);
    }
    Neff = 1/wsqrd.sum();

    int len = w.size();
    keep.resize(len);
    for (int i=0; i<len; i++) {
        keep[i] = -1;
    }

    vector<float> select;
    stratified_random(len,select); 
    cumsum(w);    

    int ctr=0;
    for (int i=0; i<len; i++) {
        while ((ctr<len) && (select[ctr]<w(i))) {
            keep[ctr] = i;
            ctr++;
        }
    }
}

//
//returns a cumulative sum array
//
void cumsum(VectorXf &w) 
{
    VectorXf csumVec(w.size());
    for (int i=0; i< w.size(); i++) {
        float sum =0;
        for (int j=0; j<=i; j++) {
            sum+=w(j);
        }			
        csumVec(i) = sum;
    }
    w = VectorXf(csumVec); //copy constructor. Double check
}
