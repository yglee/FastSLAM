#include "feature_update.h"
#include <iostream>

using namespace std;

//z is the list of measurements conditioned on the particle.
void feature_update(Particle &particle, vector<VectorXf> z, vector<int>idf, MatrixXf R)
{
    //Having selected a new pose from the proposal distribution, this pose is assumed perfect and each feature update maybe computed independently and without pose uncertainty
    int rows = 2; //2d mean for EKF
    //MatrixXf xf(rows,idf.size()); //idf.size = num measurements for landmarks   
	vector<VectorXf> xf; //updated EKF means
    vector<MatrixXf> Pf; //updated EKF covariances

	for (unsigned i=0; i<idf.size(); i++) {
		xf.push_back(particle.xf()[idf[i]]); //means
		Pf.push_back(particle.Pf()[idf[i]]); //covariances
	}	

//    unsigned i,r; 
//    for (i=0; i<idf.size(); i++) {
//        for (r=0; r<rows; r++) {
//            xf(r,i) = (particle.xf())(r,(idf[i]));	
//        }
//        Pf.push_back((particle.Pf())[idf[i]]); //particle.Pf is a array of matrices
//    }

    //MatrixXf zp //(z.rows(), idf.size());    
	//zp.setZero();  
	vector<VectorXf> zp;
    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    
	compute_jacobians(particle,idf,R,zp,&Hv,&Hf,&Sf);
    //MatrixXf v = z-zp;

	vector<VectorXf> v; //difference btw two measurements (used to update mean)
	for (int i=0; i<z.size(); i++) {
		VectorXf measure_diff = z[i] - zp[i];
		measure_diff[1] = pi_to_pi(measure_diff[1]);
		v.push_back(measure_diff);
	}

    //unsigned c;
    //for (c=0; c<v.cols();c++) {
    //    v(1,c) = pi_to_pi(v(1,c));
    //}

    VectorXf vi; //(v.rows());
    MatrixXf Hfi;
    MatrixXf Pfi;
    VectorXf xfi; //(xf.rows());

	for (int i=0; i<idf.size(); i++) {
		vi = v[i];
		Hfi = Hf[i];
		Pfi = Pf[i];
		xfi = xf[i];
        KF_cholesky_update(xfi,Pfi,vi,R,Hfi);
		xf[i] = xfi;
		Pf[i] = Pfi;
	}

#if 0
    for (i=0; i< idf.size(); i++) {
        for (r=0; r<v.rows(); r++) {
            vi(r) = v(r,i); //v.column(i);
        }
        Hfi = Hf[i];  
        Pfi = Pf[i];
        for (r=0; r<xf.rows(); r++) {
            xfi(r) = xf(r,i); //xfi = xf.column(i);
        }
        KF_cholesky_update(xfi,Pfi,vi,R,Hfi);
        
        for (r=0; r<xf.rows(); r++) {
            xf(r,i) = xfi(r);
        }
        Pf[i] = Pfi; 
    }
#endif
	for (int i=0; i<idf.size(); i++) {
		particle.setXfi(idf[i],xf[i]);
		particle.setPfi(idf[i],Pf[i]);
	}
    //particle.setXf(xf);
    //particle.setPf(Pf);
}
