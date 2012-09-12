#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>
#include "assert.h"

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, vector<VectorXf> z, vector<int> idf, MatrixXf R)
{
    VectorXf xv = VectorXf(particle.xv()); //robot position
    MatrixXf Pv = MatrixXf(particle.Pv()); //controls (motion command)

    VectorXf xv0 = VectorXf(xv);
    MatrixXf Pv0 = MatrixXf(Pv);	

    //cout<<"xv"<<endl;
    //cout<<xv<<endl;
    //cout<<"Pv"<<endl;
    //cout<<Pv<<endl;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    vector<VectorXf> zp;
    
    VectorXf zpi;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;

    //VectorXf vi(z.rows());
    vector<VectorXf> vi;

    //process each feature, incrementally refine proposal distribution
    unsigned i,r;
    for (i =0; i<idf.size(); i++) {
        vector<int> j;
        j.push_back(idf[i]);
	
        compute_jacobians(particle,j,R,zp,&Hv,&Hf,&Sf);

	zpi = zp[i];
        Hvi = Hv[i];
        Hfi = Hf[i];
        Sfi = Sf[i].inverse();

	VectorXf vi = z[i] - zpi;
	vi[1] = pi_to_pi(vi[1]);
	
        //proposal covariance
        MatrixXf Pv_inv = Pv.llt().solve(MatrixXf::Identity(Pv.rows(), Pv.cols())); 
        Pv = Hvi.transpose() * Sfi * Hvi + Pv_inv;
        Pv = Pv.llt().solve(MatrixXf::Identity(Pv.rows(), Pv.cols()));//Pv.inverse();

        //proposal mean
        xv = xv + Pv * Hvi.transpose() * Sfi * vi;
        particle.setXv(xv);
        particle.setPv(Pv); 
    }

    //sample from proposal distribution
    VectorXf xvs = multivariate_gauss(xv,Pv,1); 
    //cout<<"xvs"<<endl;
    //cout<<xvs<<endl;
    particle.setXv(xvs);
    MatrixXf zeros(3,3);
    zeros.setZero();
    particle.setPv(zeros);

    //compute sample weight: w = w* p(z|xk) p(xk|xk-1) / proposal
    //cout<<"likelihood given xv input"<<endl;
    //cout<<"particle.w() "<<particle.w()<<endl;
    //cout<<"particle.xv()"<<endl;
    //cout<<particle.xv()<<endl;
    //cout<<"particle.Pv()"<<endl;
    //cout<<particle.Pv()<<endl;
    //cout<<"particle.xf()"<<endl;
    for (int i =0; i< particle.xf().size(); i++) {
	//cout<<particle.xf()[i]<<endl;
    } 
    //cout<<endl;
    //cout<<"particle.Pf()"<<endl;
    for (int i =0; i< particle.Pf().size(); i++) {
	//cout<<particle.Pf()[i]<<endl;
    } 
    //cout<<endl;
    //cout<<"z"<<endl;
    for (int i =0; i< z.size(); i++) {
	//cout<<z[i]<<" "<<endl;
    }
    //cout<<"idf"<<endl;
    for (int i =0; i< idf.size(); i++) {
	//cout<<idf[i]<<" "<<endl;
    } 
    //cout<<endl;
    //cout<<"R"<<endl;
    //cout<<R<<endl;


    float like = likelihood_given_xv(particle, z, idf, R);
    //cout<<"like "<<like<<endl;
    float prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0,0);
    //cout<<"prior "<<prior<<endl;
    float prop = gauss_evaluate(delta_xv(xv,xvs),Pv,0);
    //cout<<"prop "<<prop<<endl;

    float newW = particle.w() * like * prior / prop;

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
	
	cout<<"zp"<<endl;
	for (int i =0; i< zp.size(); i++) {
	    cout<<zp[i]<<" "<<endl;
	}

	cout<<"Hv"<<endl;
	for (int i =0; i< Hv.size(); i++) {
	    cout<<Hv[i]<<" "<<endl;
	}

	cout<<"Hf"<<endl;
	for (int i =0; i< Hf.size(); i++) {
	    cout<<Hf[i]<<" "<<endl;
	}

	cout<<"Sf"<<endl;
	for (int i =0; i< Sf.size(); i++) {
	    cout<<Sf[i]<<" "<<endl;
	} 

	cout<<"z["<<j<<"] "<<z[j]<<endl;
	v = z[j] - zp[0];
	cout<<"v "<<v<<endl;
        v(1) = pi_to_pi(v(1));
	cout<<"v "<<v<<endl;
        w = w*gauss_evaluate(v,Sf[0],0);
	cout<<"w "<<w<<endl;
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
