#include <iostream>
#include "particles.h"

Particles::Particles(int numParticles)
:_particles(NULL)
{
	using namespace boost::numeric::ublas;

	_particles = new Particle[numParticles];
	for (int i = 0; i < numParticles; i++) {
		_particles[i].w = 1/numParticles; 
		vector<double> v (3);
		for (unsigned j = 0; j < v.size(); ++j) {
			v(j) = 0;	
		}  
		_particles[i].xv =  v;//change this to vector of 0
		_particles[i].Pv = NULL; //change this to matrix of 0
		_particles[i].xf = [];
		_particles[i].Pf = [];
		_particles[i].da = [];
	}
}

Particles::~Particles()
{
	if (_particles)
		delete [] _particles;
}

Particles::GetParticles(int index)
{
	return _particles[index];
}
