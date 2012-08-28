#include "resample_particles.h"
#include "configfile.h"
#include "stratified_resample.h"
#include <iostream>

using namespace std;

void resample_particles(vector<Particle> &particles, int Nmin, int doresample) 
{
    unsigned int N = particles.size();
    //assert(particles.size() == config::NPARTICLES);
    VectorXf w(N);
    w.setZero();

    int i;    
    for (i=0; i<N; i++) {
        w(i) = particles[i].w();
        //cout<<"w at "<<i<<" "<<w(i)<<endl;
    }

    float ws = w.sum();

    for (i=0; i<N; i++) {
        w(i) = w(i)/ws;
    }

    float Neff=0;
    vector<int> keep;
    stratified_resample(w,keep,Neff);

    vector<Particle> old_particles = vector<Particle>(particles);
    particles.resize(keep.size());	

    if ((Neff < Nmin) && (doresample == 1)) {
        for(i=0; i< keep.size(); i++) {
            particles[i] = old_particles[keep[i]]; 	
        }	
        for (i=0; i<N; i++) {
            cout<<"N"<<endl;
            cout<<N<<endl;
			float new_w = 1.0f/(float)N;
            particles[i].setW(new_w);
        }
    }		
}
