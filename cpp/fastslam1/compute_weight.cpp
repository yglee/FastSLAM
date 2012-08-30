#include "compute_weight.h"
#include <Eigen/LU>
#include "core/pi_to_pi.h"
#include "core/compute_jacobians.h"
#include <iostream>
#include <vector>

#define pi 3.1416

using namespace std;
//
//compute particle weight for sampling
//
float compute_weight(Particle &particle, vector<VectorXf> z, vector<int> idf, MatrixXf R) 
{
    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
	vector<VectorXf> zp;     

    //process each feature, incrementally refine proposal distribution
    compute_jacobians(particle,idf,R,zp,&Hv,&Hf,&Sf);

    vector<VectorXf> v;

	for (int j =0; j<z.size(); j++) {
		VectorXf v_j = z[j] - zp[j];
		v_j[1] = pi_to_pi(v_j[1]);
		v.push_back(v_j);
	}

    float w = 1.0;
    
    MatrixXf S;
    float den, num;
    for (int i=0; i<z.size(); i++) {
        S = Sf[i];
        den = 2*pi*sqrt(S.determinant());
        num = std::exp(-0.5 * v[i].transpose() * S.inverse() * v[i]);
        w = w* num/den; 
    }
    return w;
} 
