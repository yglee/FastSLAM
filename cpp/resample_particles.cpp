#include "resample_particles.h"
#include "configfile.h"

void resample_particles(Particle *particles, int Nmin, int doresample) 
{
    unsigned int N = NPARTICLES;
    VectorXf w;
    w.resize(N);

    float ws=0;
    int i;    
    for (i=0; i<N; i++) {
        w(i) = particles[i].w(); 
        ws+=w(i);
    }
    for (i=0; i<N; i++) {
        w(i) = w(i)/ws;
    }
    
    stratified_resample(w,keep,Neff);
    //TODO
}
