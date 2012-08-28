#include <iostream>
#include "particle.h"
#include <algorithm>

/*************
** Particle
*************/

Particle::Particle() 
{
	_w = 1.0; 
	_xv = VectorXf(3);
        _xv.setZero();
	_Pv = MatrixXf(3,3);
        _Pv.setZero();
	_da = NULL;
}

Particle::Particle(float &w, VectorXf &xv, MatrixXf &Pv, vector<VectorXf> &xf, vector<MatrixXf> &Pf, float* da)
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

vector<VectorXf> Particle::xf() const
{
	return _xf;	
}

vector<MatrixXf> Particle::Pf() const
{
	return _Pf;	
}

float* Particle::da() const
{
	return _da;	
}

//setters
void Particle::setW(float &w)
{
	_w = w;
}

void Particle::setXv(VectorXf &xv)
{
	_xv = xv;
}

void Particle::setPv(MatrixXf &Pv)
{
	_Pv = Pv;
}

void Particle::setXf(vector<VectorXf> &xf)
{
	_xf = xf;
}

void Particle::setXfi(int i, VectorXf &vec) 
{
	if (i >= _xf.size()){
		_xf.resize(i+1);
	}
	_xf[i] = vec;
}

void Particle::setPf(vector<MatrixXf> &Pf)
{
	_Pf = Pf;
}

void Particle::setPfi(int i, MatrixXf &m) 
{
	if(i >= _Pf.size()) {
		_Pf.resize(i+1);
	}
	_Pf[i] = m;
}

void Particle::setDa(float* da)
{
	_da = da;
}

