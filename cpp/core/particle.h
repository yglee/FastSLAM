#ifndef PARTICLES_H
#define PARTICLES_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

class Particle{
public:
	Particle();
	Particle(float w, VectorXf &xv, MatrixXf &Pv, vector<VectorXf> &xf, vector<MatrixXf> &Pf, float* da);
	~Particle();
        
	//getters	
	float w() const;
	VectorXf xv() const; //robot pose: x,y,theta (heading dir)
	MatrixXf Pv() const; //controls: velocities
	vector<VectorXf> xf() const; //2d means of EKF
	vector<MatrixXf> Pf() const; //covariance matrices for EKF 
	float* da() const; //

	//setters
	void setW(float w);
	void setXv(VectorXf &xv);
	void setPv(MatrixXf &Pv);
	void setXf(vector<VectorXf> &xf);
	void setXfi(int i, VectorXf &vec);
	void setPf(vector<MatrixXf> &Pf);
	void setPfi(int i, MatrixXf &m);
	void setDa(float* da);
	
private:
	float _w;
	VectorXf _xv;
	MatrixXf _Pv;		
	vector<VectorXf> _xf;
	vector<MatrixXf> _Pf;
	float* _da;
};

#endif //PARTICLES_H
