#include "sample_proposal.h"

#include <iostream>

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
		VectorXf xv = particle.xv();
		MatrixXf Pv = particle.Pv();
		VectorXf xv0 = VectorXf(xv);
		MatrixXf Pv0 = MatrixXf(Pv);	

		MatrixXf *Hv;
		MatrixXf *Hf;
		MatrixXf *Sf;

		MatrixXf zpi;
		MatrixXf Hvi;
		MatrixXf Hfi;
		MatrixXf Sfi;

		MatrixXf vi (z.rows(),1);

		//process each feature, incrementally refine proposal distribution
		for (int i =0; i<idf.size(); i++) {
			vector<int> j;
			j.push_back(idf[i]);
			compute_jacobians(particle,j,R,zpi,Hv,Hf,Sf);
			MatrixXf Sfi = Sf.inverse();
			Hvi = Hv[0];
			Hfi = Hf[0];
			Sfi = Sf[0];
			
			vi = z.conservativeResize(z.rows(),1) - zpi;
			vi(1,0) = pi_to_pi(vi(1,0)); 		
		
			//proposal covariance
			Pv = Hvi.transpose() * Sfi * Hvi + Pv.inverse();
			Pv = Pv.inverse();
				 	
			//proposal mean
			
		}
		
}  
