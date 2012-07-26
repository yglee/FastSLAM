#include <iostream>
#include "particles.h"

/*************
** Particle
*************/

Particle::Particle() 
{
	_w = 1.0; 
	_xv.setZero();
	_Pv.setZero();
	_xf = NULL;
	_Pf = NULL;
	_da = NULL;
}

Particle::Particle(float w, Vector3f xv, Matrix3f Pv, float* xf, float* Pf, float* da)
{
	_w = w;
	_xv = xv;
	_Pv = Pv;
	_xf = xf;
	_Pf = Pf;
	_da = da;		
}

Particle::~Particle()
{
}

//getters
float Particle::w() const
{
	return _w;	
}

Vector3f Particle::xv() const
{
	return _xv;	
}

Matrix3f Particle::Pv() const
{
	return _Pv;	
}

float* Particle::xf() const
{
	return _xf;	
}

float* Particle::Pf() const
{
	return _Pf;	
}

float* Particle::da() const
{
	return _da;	
}

//setters
void Particle::setW(float w)
{
	_w = w;
}

void Particle::setXv(Vector3f xv)
{
	for (int s=0; s<xv.size(); s++) {
		_xv(s) = xv(s); //copy over the values
	}
}

void Particle::setPv(Matrix3f Pv)
{
	for (int r=0; r<Pv.rows(); r++){
		for (int j=0; j<Pv.cols(); j++){
			_Pv(r,j) = Pv(r,j); //copy over the values
		}
	}
}

void Particle::setXf(float* xf)
{
	_xf = xf;
}

void Particle::setPf(float* Pf)
{
	_Pf = Pf;
}

void Particle::setDa(float* da)
{
	_da = da;
}


/*************
** Particles
*************/
Particles::Particles(unsigned numParticles)
:_particles(NULL)
{
	_particles = new Particle[numParticles];

	//TODO: can i get rid of this loop?
	for (int i = 0; i < numParticles; i++) {
		_particles[i].setW(1.0/(float) numParticles);
	}
}

Particles::~Particles()
{
	if (_particles)
		delete [] _particles;
}

//getters
Particle &Particles::getParticle(unsigned index) const
{
	return _particles[index];
}

//setters
void Particles::setParticle(unsigned index, Particle &p) 
{
	_particles[index] = p; 
}


