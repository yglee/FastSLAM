#include "stratified_random.h"
#include <assert.h>
#include <iostream>

using namespace std;

void stratified_random(int N, vector<float> &di)
{ 
    float k = 1.0/(float)N;
   
    //deterministic intervals
    float temp = k/2;
    di.push_back(temp);
    while (temp < 1-k/2) {
        temp = temp+k;
        di.push_back(temp);
    }
    /* 
    cout<<"di"<<endl;
    for (int i=0; i< di.size(); i++) {
	cout<<di[i]<<" "<<endl;
    }
 
    cout<<"N "<<N<<endl;
    cout<<"di size "<<di.size()<<endl; 
    //assert(di.size() == N); 
    */
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
