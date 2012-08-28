#include "compute_weight.h"
#include <Eigen/LU>
#include "core/pi_to_pi.h"
#include "core/compute_jacobians.h"
#include <iostream>
#include <vector>

#define pi 3.1416
using namespace std;

//float compute_weight(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R) 
float compute_weight(Particle &particle, vector<VectorXf> z, vector<int> idf, MatrixXf R) 
{
    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    //MatrixXf zp(z.rows(),z.cols());
	vector<VectorXf> zp;     

    //process each feature, incrementally refine proposal distribution
    compute_jacobians(particle,idf,R,zp,&Hv,&Hf,&Sf);

    //MatrixXf v(z.rows(),z.cols());
    vector<VectorXf> v;

	for (int j =0; j<z.size(); j++) {
		//v[j] = z[j] - zp[j]; //measurement difference
		VectorXf v_j = z[j] - zp[j];
		//v[j][1] = pi_to_pi(v[j][1]);
		v_j[1] = pi_to_pi(v_j[1]);
		v.push_back(v_j);
	}
	//unsigned r,c;
    //for (r=0; r<z.rows(); r++) {
    //    for (c=0; c<z.cols(); c++) {        
    //        v(r,c) = z(r,c) - zp(r,c);
    //    }
    //}
    
    //for (int c=0; c<v.cols(); c++) {
    //    v(1,c) = pi_to_pi(v(1,c));
    //}

    float w = 1.0;
    
    MatrixXf S;
    float den, num;
    //VectorXf vcol(v.rows()); 
    //VectorXf vcol(v.size()); 
    //for (int i=0; i<z.cols(); i++) {
    for (int i=0; i<z.size(); i++) {
        S = Sf[i];
        den = 2*pi*sqrt(S.determinant());
        //for (int r=0; r< v.rows(); r++) {
        //for (int r=0; r< v.size(); r++) {
        //    vcol[r] = v(r,i);
        //}
        num = std::exp(-0.5 * v[i].transpose() * S.inverse() * v[i]);
        w = w* num/den; 
    }
    return w;
} 
