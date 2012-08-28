#include "feature_update.h"
#include <iostream>

using namespace std;

//z is the list of measurements conditioned on the particle.
void feature_update(Particle &particle, vector<VectorXf> z, vector<int>idf, MatrixXf R)
{
    //Having selected a new pose from the proposal distribution, this pose is assumed perfect and each feature update maybe computed independently and without pose uncertainty
    int rows = 2; //2d mean for EKF
	vector<VectorXf> xf; //updated EKF means
    vector<MatrixXf> Pf; //updated EKF covariances

	for (unsigned i=0; i<idf.size(); i++) {
		xf.push_back(particle.xf()[idf[i]]); //means
		Pf.push_back(particle.Pf()[idf[i]]); //covariances
	}	
	
	vector<VectorXf> zp;
    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    
	compute_jacobians(particle,idf,R,zp,&Hv,&Hf,&Sf);

	vector<VectorXf> v; //difference btw two measurements (used to update mean)
	for (int i=0; i<z.size(); i++) {
		VectorXf measure_diff = z[i] - zp[i];
		measure_diff[1] = pi_to_pi(measure_diff[1]);
		v.push_back(measure_diff);
	}

    VectorXf vi; 
    MatrixXf Hfi;
    MatrixXf Pfi;
    VectorXf xfi; 

	for (int i=0; i<idf.size(); i++) {
		vi = v[i];
		Hfi = Hf[i];
		Pfi = Pf[i];
		xfi = xf[i];
        KF_cholesky_update(xfi,Pfi,vi,R,Hfi);
		xf[i] = xfi;
		Pf[i] = Pfi;
	}

	for (int i=0; i<idf.size(); i++) {
		particle.setXfi(idf[i],xf[i]);
		particle.setPfi(idf[i],Pf[i]);
	}
}
