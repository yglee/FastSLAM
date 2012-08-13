#include "resample_particles.h"
#include "configfile.h"
#include "stratified_resample.h"

void resample_particles(vector<Particle> &particles, int Nmin, int doresample) 
{
    unsigned int N = particles.size();
	assert(particles.size() == config::NPARTICLES);
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
    
	float Neff;
	vector<int> keep;
    stratified_resample(w,keep,Neff);

	vector<Particle> old_particles = vector<Particle>(particles);
	particles.resize(keep.size());	
	
	if ((Neff < Nmin) && (doresample == 1)) {
		for(i=0; i< keep.size(); i++) {
			particles[i] = old_particles[keep[i]]; 	
		}	
		for (i=0; i<N; i++) {
			particles[i].setW(1/N);
		}
	}		
}
