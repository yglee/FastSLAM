#ifndef PARTICLES_H
#define PARTICLES_H

#include <boost/numeric/ublas/vector.hpp> 
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas = boost::numeric::ublas;

struct Particle{
	float w;
	ublas::vector<float> xv;
	ublas::matrix<float> Pv;
	float* xf;
	float* Pf;
	float* da;
};

class Particles
{
public:
	Particles(unsigned numParticles);
	~Particles();	
	Particle GetParticle(unsigned index);
private:	
	Particle *_particles;
};

#endif //PARTICLES_H
