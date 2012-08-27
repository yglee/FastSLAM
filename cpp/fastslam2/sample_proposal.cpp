#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>
#include "assert.h"

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
    VectorXf xv(particle.xv()); //robot position
    MatrixXf Pv(particle.Pv()); //controls (motion command)

    VectorXf xv0(xv);
    MatrixXf Pv0(Pv);	

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zpi(z.rows(),1);
    zpi.setZero();
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;

    VectorXf vi(z.rows());

    //process each feature, incrementally refine proposal distribution
    unsigned i,r;
    for (i =0; i<idf.size(); i++) {
        vector<int> j;
        j.push_back(idf[i]);

        compute_jacobians(particle,j,R,zpi,&Hv,&Hf,&Sf);
        assert(zpi.cols() == 1);

        Hvi = Hv[i];
        Hfi = Hf[i];
        Sfi = Sf[i].inverse();

        for (r=0; r<z.rows(); r++) {
            vi[r] = z(r,i) - zpi(r,0);
        }

        vi[1] = pi_to_pi(vi[1]);

        //proposal covariance
        MatrixXf Pv_inv = Pv.llt().solve(MatrixXf::Identity(Pv.rows(), Pv.cols())); 
        Pv = Hvi.transpose() * Sfi * Hvi + Pv_inv;//Pv.inverse();
        Pv = Pv.llt().solve(MatrixXf::Identity(Pv.rows(), Pv.cols()));//Pv.inverse();

        //proposal mean
        xv = xv + Pv * Hvi.transpose() * Sfi *vi;
        particle.setXv(xv);
        particle.setPv(Pv); 
    }

    //sample from proposal distribution
    VectorXf xvs = multivariate_gauss(xv,Pv,1); 
    particle.setXv(xvs);
    MatrixXf zeros(3,3);
    zeros.setZero();
    particle.setPv(zeros);

    //compute sample weight: w = w* p(z|xk) p(xk|xk-1) / proposal
    float like = likelihood_given_xv(particle, z, idf, R);
    float prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0,0); 
    float prop = gauss_evaluate(delta_xv(xv,xvs),Pv,0);

    particle.setW(particle.w() * like * prior / prop); 
} 

float likelihood_given_xv(Particle particle, MatrixXf z, vector<int>idf, MatrixXf R) 
{
    float w = 1;
    vector<int> idfi;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    
    MatrixXf zp;
    MatrixXf Sfi;
    VectorXf v(z.rows());    

    unsigned i,k;
    
    for (i=0; i<idf.size(); i++){
        idfi.push_back(idf[i]);
        zp.resize(z.rows(), idfi.size());
        compute_jacobians(particle,idfi,R,zp,&Hv,&Hf,&Sf);

        for (k=0; k<z.rows(); k++) {
            v(k) = z(k,i)-zp(k,i); 
        }
        v(1) = pi_to_pi(v(1));

        w = w*gauss_evaluate(v,Sf[0],0);
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

