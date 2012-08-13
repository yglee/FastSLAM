#include <iostream>
#include "particle.h"
#include <algorithm>

/*************
** Particle
*************/

Particle::Particle() 
{
	_w = 1.0; 
	_xv = VectorXf(3,1);
	_Pv = MatrixXf(3,3);
        _Pv.setZero();
	_xf = MatrixXf(0,0);
	_Pf = NULL;
	_da = NULL;
}

Particle::Particle(float w, VectorXf xv, MatrixXf Pv, MatrixXf xf, MatrixXf* Pf, float* da)
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

VectorXf Particle::xv() const
{
	return _xv;	
}

MatrixXf Particle::Pv() const
{
	return _Pv;	
}

MatrixXf Particle::xf() const
{
	return _xf;	
}

MatrixXf* Particle::Pf() const
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

void Particle::setXv(VectorXf xv)
{
	for (int s=0; s<xv.size(); s++) {
		_xv(s) = xv(s); //copy over the values
	}
}

void Particle::setPv(MatrixXf Pv)
{
	for (int r=0; r<Pv.rows(); r++){
		for (int j=0; j<Pv.cols(); j++){
			_Pv(r,j) = Pv(r,j); //copy over the values
		}
	}
}

void Particle::setXf(MatrixXf xf)
{
	_xf = xf;
}

void Particle::setPf(MatrixXf* Pf)
{
	_Pf = Pf;
}

void Particle::setDa(float* da)
{
	_da = da;
}

