#include <iostream>
#include "compute_jacobians.h"
#include "math.h"
#include "pi_to_pi.h"

void compute_jacobians(Particle particle, 
		vector<int> idf, 
		MatrixXf R, 
		vector<VectorXf> &zp, //measurement (range, bearing)
		vector<MatrixXf> *Hv, // jacobians of function h (deriv of h wrt pose)
		vector<MatrixXf> *Hf, // jacobians of function h (deriv of h wrt mean)
		vector<MatrixXf> *Sf) //measurement covariance
{
	VectorXf xv = particle.xv();

	int rows = particle.xf().size();
	vector<VectorXf> xf;
	vector<MatrixXf> Pf;

	unsigned i;
	int r;
	for (unsigned i=0; i<idf.size(); i++) {
		xf.push_back(particle.xf()[idf[i]]);
		Pf.push_back((particle.Pf())[idf[i]]); //particle.Pf is a array of matrices
	}

	float dx,dy,d2,d;
	MatrixXf HvMat(2,3);
	MatrixXf HfMat (2,2);

	for (i=0; i<idf.size(); i++) {
		dx = xf[i](0) - xv(0);
		dy = xf[i](1) - xv(1);
		d2 = pow(dx,2) + pow(dy,2);	
		d = sqrt(d2);

		VectorXf zp_vec(2);
		
		//predicted observation
		zp_vec[0] = d;
		zp_vec[1] = atan2(dy,dx) - xv(2);
		zp_vec[1] = pi_to_pi(zp_vec[1]);
		zp.push_back(zp_vec);
		
		//Jacobian wrt vehicle states
		HvMat<< -dx/d, -dy/d,  0, 
			dy/d2, -dx/d2,-1;

		//Jacobian wrt feature states
		HfMat<< dx/d,  dy/d,
			-dy/d2, dx/d2;

		Hv->push_back(HvMat);
		Hf->push_back(HfMat);

		//innovation covariance of feature observation given the vehicle'
		MatrixXf SfMat = HfMat*Pf[i]*HfMat.transpose() + R; 
		Sf->push_back(SfMat);      
	}			
}
