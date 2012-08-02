#include "sample_proposal.h"

#include <iostream>

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
	VectorXf xv = particle.xv();
	MatrixXf Pv = particle.Pv();
	VectorXf xv0 = VectorXf(xv);
	MatrixXf Pv0 = MatrixXf(Pv);	
		
        MatrixXf zp(z.rows(),idf.size());
        MatrixXf *Hv;
        MatrixXf *Hf;
        MatrixXf *Sf;
        
        MatrixXf zpi;
        MatrixXf Hvi;
        MatrixXf Hfi;
        MatrixXf Sfi;
        
	//process each feature, incrementally refine proposal distribution
        for (int i =0; i<idf.size(); i++) {
		vector<int> j;
                j.push_back(idf[i]);
		compute_jacobians(particle,j,R,zp,Hv,Hf,Sf);
                MatrixXf Sfi = Sf.inverse();
                zpi = zp[0];
                Hvi = Hv[0];
                Hfi = Hf[0];
                Sfi = Sf[0];
	}
}  
