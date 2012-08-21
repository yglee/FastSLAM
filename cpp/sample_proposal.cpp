#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>
#include "assert.h"

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
    VectorXf xv(particle.xv());
    MatrixXf Pv(particle.Pv());

    VectorXf xv0(xv);
    MatrixXf Pv0(Pv);	

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zpi;
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
#if 1 
        cout<<"i in SAMPLE_PROPOSAL"<<endl;
        cout<<i<<endl;
        cout<<"should be 1"<<endl;
        cout<<endl;

        cout<<"z"<<endl;
        cout<<z<<endl;
        cout<<endl;
        cout<<"zpi"<<endl;
        cout<<zpi<<endl;
#endif
        for (r=0; r<z.rows(); r++) {
            cout<<"z("<<r<<","<<i<<") "<<z(r,i)<<endl;;
            cout<<"zpi("<<r<<","<<"0) "<<zpi(r,0)<<endl;;
            vi[r] = z(r,i) - zpi(r,0);
            cout<<"vi["<<r<<"] "<<vi[r]<<endl; 
        }
#if 1
        cout<<"vi"<<endl;
        cout<<vi<<endl;
        cout<<"should be [0.1355; -0.0333]"<<endl; 
        cout<<endl;
#endif
        vi[1] = pi_to_pi(vi[1]);

        //add a little bias so I won't get NaNs when I do an inverse

        cout<<"Pv"<<endl;
        cout<<Pv<<endl;

        #if 0 
        for (r=0; r<Pv.rows(); r++) {
            for (c=0; c<Pv.cols(); c++) {
                if (r==c) {
                    Pv(r,c) += 1.0e-6;
                } 
            }   
        }
        #endif

        //proposal covariance
        Pv = Hvi.transpose() * Sfi * Hvi + Pv.inverse();
        Pv = Pv.inverse();

        //proposal mean
        xv = xv + Pv * Hvi.transpose() * Sfi *vi;
        cout<<"xv at the end"<<endl;
        cout<<xv<<endl;
        particle.setXv(xv);
        particle.setPv(Pv); 
    }

    //sample from proposal distribution
    VectorXf xvs = multivariate_gauss(xv,Pv,1); 
    particle.setXv(xvs);
    particle.setPv(MatrixXf(3,3));

    //compute sample weight: w = w* p(z|xk) p(xk|xk-1) / proposal
    float like = likelihood_given_xv(particle, z, idf, R);
    float prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0,0); 
    float prop = gauss_evaluate(delta_xv(xv,xvs),Pv,0);
    #if 1
    cout<<"in compute_jacob: particle.w() = "<<particle.w()<<endl;
    cout<<"like = "<<like<<endl;
    cout<<"prior = "<<prior<<endl;
    cout<<"prop = "<<prop<<endl;
    #endif
    particle.setW(particle.w() * like * prior / prop); 
    #if 1
    cout<<"particle.w()"<<endl;
    cout<<particle.w()<<endl;
    #endif
} 

float likelihood_given_xv(Particle particle, MatrixXf z, vector<int>idf, MatrixXf R) 
{
    float w = 1;
    vector<int> idfi;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zp;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;
    VectorXf v(z.rows());    

    unsigned i,k;
    //cout<<"idf.size() "<<idf.size()<<endl;
    for (i=0; i<idf.size(); i++){
        idfi.push_back(i);
        compute_jacobians(particle,idfi,R,zp,&Hv,&Hf,&Sf);
        Hvi = Hv[0]; 
        Hfi = Hf[0]; 
        Sfi = Sf[0];

        for (k=0; k<z.rows(); k++) {
            v(k) = z(k,i)-zp(k,0); //TODO: this returns wrong values
        }
        v(1) = pi_to_pi(v(1));

        w = w*gauss_evaluate(v,Sf[i],0);
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

