#include <iostream>
#include "particles.h"

Particles::Particles(unsigned numParticles)
:_particles(NULL)
{
	_particles = new Particle[numParticles];

	//for each particle
	for (unsigned i = 0; i < numParticles; i++) {
		//weight is uniform
		_particles[i].w = 1.0/(float)numParticles; 
		//3d zero-vector
		_particles[i].xv.setZero();
		//3x3 zero-matrix
		_particles[i].Pv.setZero();
		_particles[i].xf = NULL;
		_particles[i].Pf = NULL;
		_particles[i].da = NULL;
	}
}

Particles::~Particles()
{
	if (_particles)
		delete [] _particles;
}

Particle Particles::GetParticle(unsigned index)
{
	return _particles[index];
}
