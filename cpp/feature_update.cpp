#include "feature_update.h"

void feature_update(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R)
{
   //Having selected a new pose from the proposal distribution, this pose is assumed perfect and each feature update maybe computed independently and without pose uncertainty

    int rows = (particle.xf()).rows();
    MatrixXf xf(rows,idf.size());    
    MatrixXf *Pf;
    for (int i=0; i<idf.size(); i++) {
        xf.setColumn(i,((particle.xf()).column(idf(i))));
        Pf[c] = (particle.Pf())[idf[c]];
    }
    
    MatrixXf zp;
    MatrixXf *Hv;
    MatrixXf *Hf;
    MatrixXf *Sf;
    compute_jacobians(particle,idf,R,zp,Hv,Hf,Sf);
    MatrixXf v = z-zp; 
    v.setRow(1,pi_to_pi(v.row(1)));

    VectorXf vi;
    MatrixXf Hfi;
    MatrixXf Pfi;
    VectorXf xfi;

    for (int i=0; i< idf.size(); i++) {
        vi = v.column(i);
        Hfi = Hf[i];  
        Pfi = Pf[i];
        xfi = xf.column(i);

        KF_cholesky_update(xfi,Pfi,vi,R,Hfi,xf,Pf,i);
    }
    //TODO: finish this
}
