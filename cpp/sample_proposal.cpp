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

	cout<<"particle attributes"<<endl;
	cout<<"w"<<particle.w()<<endl;
	cout<<"xv"<<particle.xv()<<endl;
	
	cout<<"Pv"<<particle.Pv()<<endl;
	//TODO: Pv should be set at observe_heading or someplace else 
	//before it gets here. It is not initialized anywhere by the time 
	//it arrives at sample_proposal

	cout<<"xf"<<particle.xf()<<endl;
	cout<<"Pf"<<(particle.Pf())[0]<<endl;
	
    //process each feature, incrementally refine proposal distribution
	unsigned i;
    for (i =0; i<idf.size(); i++) {
        vector<int> j;
        j.push_back(idf[i]);
        compute_jacobians(particle,j,R,zpi,Hv,Hf,Sf);
        Hvi = Hv[i];
        Hfi = Hf[i];
        Sfi = Sf[i].inverse();

		cout<<"z rows: "<<z.rows()<<" z cols: "<<z.cols()<<endl;
		cout<<"zpi rows: "<<zpi.rows()<<" z cols: "<<zpi.cols()<<endl;
		
		vi<<1,2;
        //vi = z.conservativeResize(z.rows(),1) - zpi;
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
    float like = likelihood_given_xv(particle, z, idf, R);
    float prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0); 
    float prop = gauss_evaluate(delta_xv(xv,xvs),Pv);
    particle.setW(particle.w() * like * prior / prop); 
} 

float likelihood_given_xv(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R) 
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
    MatrixXf zi(z.rows(),1);
    MatrixXf v;    

	unsigned i,k;
    for (i=0; i<idf.size(); i++){
		temp.push_back(i);
        compute_jacobians(particle, temp, R, zp, Hv, Hf, Sf);
        Hvi = Hv[0]; 
        Hfi = Hf[0]; 
        Sfi = Sf[0];
        for (k=0; k<z.rows(); k++) {
            zi(k,i);
        }
        v = zi-zp;
        v(1,0) = pi_to_pi(v(1,0));
        w = w*gauss_evaluate(v,Sf[i]);
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
