#ifndef PARTICLES_H
#define PARTICLES_H

#include <boost/numeric/ublas/vector.hpp> 

struct Particle{
	float w;
	vector<double>* xv;
	float* Pv;
	float* xf;
	float* Pf;
	float* da;
};

class Particles
{
public:
	Particles(int numParticles);
	~Particles();	

	Particle GetParticle(int index);
private:	
	Particle *_particles;
};

#endif //PARTICLES_H
