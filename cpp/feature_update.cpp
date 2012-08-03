#include "feature_update.h"

void feature_update(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R)
{
   //Having selected a new pose from the proposal distribution, this pose is assumed perfect and each feature update maybe computed independently and without pose uncertainty

    int rows = (particle.xf()).rows();
    MatrixXf xf(rows,idf.size());    
    MatrixXf *Pf;
   
	unsigned i,r; 
    for (i=0; i<idf.size(); i++) {
		for (r=0; r<(particle.xf()).rows(); r++) {
			xf(r,i) = (particle.xf())(r,(idf[i]));	
		}
        Pf[i] = (particle.Pf())[idf[i]]; //particle.Pf is a array of matrices
    }
    
    MatrixXf zp;
    MatrixXf *Hv;
    MatrixXf *Hf;
    MatrixXf *Sf;
    compute_jacobians(particle,idf,R,zp,Hv,Hf,Sf);
    MatrixXf v = z-zp;
	
	unsigned c;
	for (c=0; c<v.cols();c++) {
		v(1,c) = pi_to_pi(v(1,c));
	}
 
    VectorXf vi;
    MatrixXf Hfi;
    MatrixXf Pfi;
    VectorXf xfi;

    for (i=0; i< idf.size(); i++) {
        for (r=0; r<v.rows(); r++) {
			vi(r) = v(r,i); //v.column(i);
        }
		Hfi = Hf[i];  
        Pfi = Pf[i];
		for (r=0; r<xf.rows(); r++) {
        	xfi(r) = xf(r,i); //xfi = xf.column(i);
		}
       // KF_cholesky_update(xfi,Pfi,vi,R,Hfi,xf,Pf,i);
    }
    //TODO: finish this
}
