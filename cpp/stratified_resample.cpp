#include "stratified_resample.h"

void stratified_resample(VectorXf w, VectorXf &keep, float &Neff)
{
    VectorXf wsqrd;
    for (int i=0; i<w.size(); i++) {
        w(i) = w(i)/w.sum();
        wsqrd(i) = pow(w(i),2);
    }
    Neff = 1/wsqrd.sum();
    int len = w.size();
    vector<float> select;
    stratified_random(len,select); 
    
    //implement cumsum
}

//
//returns a cumulative sum array
//
VectorXf cumsum(VectorXf w) 
{
}
