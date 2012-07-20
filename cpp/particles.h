#ifndef PARTICLES_H
#define PARTICLES_H

#include <eigen3/Eigen/Dense>
using namespace Eigen;

struct Particle{
	float w;
	Vector3f xv;
	Matrix3f Pv;		
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
