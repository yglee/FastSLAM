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

    MatrixXf vi(z.rows(),1);

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
        xv = xv * Pv * Hvi.transpose() * Sfi *vi;
        
        particle.setXv(xv);
        particle.setPv(Pv); 
    }
    
    //sample from proposal distribution
    VectorXf xvs = multivariate_gauss(xv,Pv,1); 
    particle.setXv(xvs);
    particle.setPv(VectorXf(3));
    
    //compute sample weight: w = w* p(z|xk) p(xk|xk-1) / proposal
    VectorXf like = likelihood_given_xv(particle, z, idf, R);
    particle.setW(particle.w() * like * prior / prop);
    VectorXf prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0); 
    VectorXf prop = gauss_evaluate(delta_xv(xv,xvs),Pv);
    particle.setW(particle.w() * like * prior / prop); 
} 

VectorXf likelihood_given_xv(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R) 
{
    float w = 1;
    vector<int> temp;

    MatrixXf *Hv;
    MatrixXf *Hf;
    MatrixXf *Sf;

    MatrixXf zp;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;
   
    int k; 
    MatrixXf zi(z.rows(),1);
    MatrixXf v;    

    for (int i=0; i<idf.size(); i++) {
        temp;
        compute_jacobians(particle, temp.push_back(i), R, zp, Hv, Hf, Sf);
        Hvi = Hv[0]; 
        Hfi = Hf[0]; 
        Sfi = Sf[0];
    
        for (k=0; k<z.rows(); k++) {
            zi(k,i);
        }
        
        v = zi-zp;
        v(1,0) = pi_to_pi(v(1,0);
        
        w = w* gauss_evaluate(v,Sf);
    } 
    return w;
}

VectorXf delta_xv(VectorXf xv1, VectorXf xv2)
{
    //compute innovation between two xv estimates, normalising the heading component
    VectorXf dx = xv1-xv2; 
    dx(2) = pi_to_pi(dx(2));
    return dx;
}
