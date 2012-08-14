#include "feature_update.h"
#include <iostream>

using namespace std;

void feature_update(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R)
{
    //Having selected a new pose from the proposal distribution, this pose is assumed perfect and each feature update maybe computed independently and without pose uncertainty

    int rows = (particle.xf()).rows();
    MatrixXf xf(rows,idf.size());    
    vector<MatrixXf> Pf;

    unsigned i,r; 
    for (i=0; i<idf.size(); i++) {
        for (r=0; r<(particle.xf()).rows(); r++) {
            xf(r,i) = (particle.xf())(r,(idf[i]));	
        }
        Pf[i] = (particle.Pf())[idf[i]]; //particle.Pf is a array of matrices
    }

    MatrixXf zp(2,idf.size());//idf.size is zero...
    vector<MatrixXf> *Hv;
    vector<MatrixXf> *Hf;
    vector<MatrixXf> *Sf;
    compute_jacobians(particle,idf,R,zp,Hv,Hf,Sf);
    MatrixXf v = z-zp; //TODO: need to fix: idf.size() is zero. which makes this break.

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
        Hfi = (*Hf)[i];  
        Pfi = Pf[i];
        for (r=0; r<xf.rows(); r++) {
            xfi(r) = xf(r,i); //xfi = xf.column(i);
        }
        KF_cholesky_update(xfi,Pfi,vi,R,Hfi);
        //TODO: insert xfi into xf, etc
        for (r=0; r<xf.rows(); r++) {
            xf(r,i) = xfi(r);
        }
        Pf[i] = Pfi; 
    }

    particle.setXf(xf);
    particle.setPf(Pf);
}
