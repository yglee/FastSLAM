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
    stratified_random(len,select); 
    //TODO
}
