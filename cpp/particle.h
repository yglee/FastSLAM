#ifndef PARTICLES_H
#define PARTICLES_H

#include <Eigen/Dense>
using namespace Eigen;

class Particle{
public:
	Particle();
	Particle(float w, VectorXf xv, MatrixXf Pv, MatrixXf xf, MatrixXf* Pf, float* da);
	~Particle();
        
	//getters	
	float w() const;
	VectorXf xv() const;
	MatrixXf Pv() const;
	MatrixXf xf() const;
	MatrixXf* Pf() const;
	float* da() const;

	//setters
	void setW(float w);
	void setXv(VectorXf xv);
	void setPv(MatrixXf Pv);
	void setXf(MatrixXf xf);
	void setPf(MatrixXf* Pf);
	void setDa(float* da);
	
private:
	float _w;
	VectorXf _xv;
	MatrixXf _Pv;		
	MatrixXf _xf;
	MatrixXf* _Pf;
	float* _da;
};

#endif //PARTICLES_H
