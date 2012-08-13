#include "stratified_random.h"
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

void stratified_random(int N, vector<float> &di)
{ 
    float k = 1/N;
   
    //deterministic intervals
    di.push_back(k/2);
    float temp = k;
    while (temp <= (1-k/2)) {
        di.push_back(temp);
        temp = temp*2;
    }
    
    assert(di.size() == N); 
    
    //dither within interval
    vector<float>::iterator diter; 
    for (diter = di.begin(); diter != di.end(); diter++) {
        *diter = (*diter) + unifRand() * k - (k/2);
    }
}

//
// Generate a random number between 0 and 1
// return a uniform number in [0,1].
double unifRand()
{
    return rand() / double(RAND_MAX);
}
