#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>
#include <math.h>
#include "assert.h"

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, vector<VectorXf> z, vector<int> idf, MatrixXf R)
{
    assert(isfinite(particle.w()));
    VectorXf xv = VectorXf(particle.xv()); //robot position
    MatrixXf Pv = MatrixXf(particle.Pv()); //controls (motion command)

    VectorXf xv0 = VectorXf(xv);
    MatrixXf Pv0 = MatrixXf(Pv);	

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    vector<VectorXf> zp;

    VectorXf zpi;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;

    //process each feature, incrementally refine proposal distribution
    unsigned i,r;
    vector<int> j;
    for (i =0; i<idf.size(); i++) {
        j.clear();
        j.push_back(idf[i]);

        Hv.clear();
        Hf.clear();
        Sf.clear();
        zp.clear();

        compute_jacobians(particle,j,R,zp,&Hv,&Hf,&Sf);

        zpi = zp[0];
        Hvi = Hv[0];
        Hfi = Hf[0];
        Sfi = Sf[0];

        VectorXf vi = z[i] - zpi;
        vi[1] = pi_to_pi(vi[1]);

        //proposal covariance
        Pv = Hvi.transpose() * Sfi * Hvi + Pv.inverse();
        Pv = Pv.inverse();

        //proposal mean
        xv = xv + Pv * Hvi.transpose() * Sfi * vi;
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
    assert(isfinite(particle.w()));

    float a = prior/prop;
    float b = particle.w() * a;
    float newW = like * b;
    //float newW = particle.w() * like * prior / prop;
    #if 0
    if (!isfinite(newW)) {
	cout<<"LIKELIHOOD GIVEN XV INPUTS"<<endl;   
	cout<<"particle.w()"<<endl;
	cout<<particle.w()<<endl;
	cout<<"particle.xv()"<<endl;
	cout<<particle.xv()<<endl;
	
	cout<<"particle.Pv()"<<endl;
	cout<<particle.Pv()<<endl;
	cout<<"particle.xf"<<endl;
	for (int i =0; i<particle.xf().size(); i++) {
	    cout<<particle.xf()[i]<<endl;
	}
	cout<<endl;
	cout<<"particle.Pf()"<<endl;
	for (int i =0; i< particle.Pf().size(); i++) {
	    cout<<particle.Pf()[i]<<endl;
	}
	cout<<endl;
	
	cout<<"z"<<endl;
	for (int i=0; i<z.size(); i++) {
	    cout<<z[i]<<endl;
	}   
	cout<<endl;
	
	cout<<"idf"<<endl;
	for (int i =0; i<idf.size(); i++){
	    cout<<idf[i]<<" ";
	}		
	cout<<endl;

	cout<<"R"<<endl;
	cout<<R<<endl;

	cout<<"GAUSS EVALUATE INPUTS"<<endl;
	cout<<"delta_xv(xv0,xvs)"<<endl;	
	cout<<delta_xv(xv0,xvs)<<endl;
	
	cout<<"Pv0"<<endl;
	cout<<Pv0<<endl;
	
	cout<<"delta_xv(xv,xvs)"<<endl;	
	cout<<delta_xv(xv,xvs)<<endl;
	
	cout<<"Pv"<<endl;
	cout<<Pv<<endl;
		
	
	cout<<"like"<<endl;
	cout<<like<<endl;
	cout<<"prior"<<endl;
	cout<<prior<<endl;
	cout<<"prop"<<endl;
	cout<<prop<<endl;
    }
    #endif
    particle.setW(newW);
} 

float likelihood_given_xv(Particle particle, vector<VectorXf> z, vector<int>idf, MatrixXf R) 
{
    float w = 1;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    vector<VectorXf> zp;

    unsigned j,k;
    vector<int> idfi;
    
    for (j=0; j<idf.size(); j++){
	idfi.clear();
        idfi.push_back(idf[j]);
	Hv.clear();
	Hf.clear();
	Sf.clear();
	zp.clear();

	VectorXf v(z.size()); 
        compute_jacobians(particle,idfi,R,zp,&Hv,&Hf,&Sf);
	v = z[j] - zp[0];
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
