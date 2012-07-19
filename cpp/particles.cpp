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
		for (unsigned t=0; t<3; t++){
			(_particles[t].xv).insert_element(t,0.0);
		}
		//3x3 zero-matrix
		for (unsigned r=0; r<3; r++) {
			for (unsigned c=0; c<3; c++) {
				//(_particles[i].Pv)(r,c) = 0.0;
				(_particles[i].Pv).insert_element(r,c,0.0);
			}
		}
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
