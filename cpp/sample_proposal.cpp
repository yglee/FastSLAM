#include "sample_proposal.h"

#include <iostream>

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
	VectorXf xv = particle.xv();
	MatrixXf Pv = particle.Pv();
	VectorXf xv0 = VectorXf(xv);
	MatrixXf Pv0 = MatrixXf(Pv);	
	
	//process each feature, incrementally refine proposal distribution
	for (int i =0; i<idf.size(); i++) {
		int j = idf[i];
		
		//compute_jacobians(particle, j, R);
	}
}  
