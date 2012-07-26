#ifndef PARTICLES_H
#define PARTICLES_H

#include <Eigen/Dense>
using namespace Eigen;

class Particle{
public:
	Particle();
	Particle(float w, Vector3f xv, Matrix3f Pv, float* xf, float* Pf, float* da);
	~Particle();
        
	//getters	
	float w() const;
	Vector3f xv() const;
	Matrix3f Pv() const;
	float* xf() const;
	float* Pf() const;
	float* da() const;

	//setters
	void setW(float w);
	void setXv(Vector3f xv);
	void setPv(Matrix3f Pv);
	void setXf(float* xf);
	void setPf(float* Pf);
	void setDa(float* da);
	
private:
	float _w;
	Vector3f _xv;
	Matrix3f _Pv;		
	float* _xf;
	float* _Pf;
	float* _da;
};

/*
class Particles
{
public:
	Particles(unsigned numParticles);
	~Particles();
	Particle &getParticle(unsigned index) const;
	void setParticle(unsigned index, Particle &p);
private:	
	Particle *_particles;
};
*/

#endif //PARTICLES_H
