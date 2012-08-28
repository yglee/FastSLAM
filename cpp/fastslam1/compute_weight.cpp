#include "compute_weight.h"
#include <Eigen/LU>
#include "core/pi_to_pi.h"
#include "core/compute_jacobians.h"
#include <iostream>

#define pi 3.1416
using namespace std;

float compute_weight(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R) 
{
    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;
    MatrixXf zp(z.rows(),z.cols());
    
    //process each feature, incrementally refine proposal distribution
    compute_jacobians(particle,idf,R,zp,&Hv,&Hf,&Sf);

    MatrixXf v(z.rows(),z.cols());
    unsigned r,c;
    for (r=0; r<z.rows(); r++) {
        for (c=0; c<z.cols(); c++) {        
            v(r,c) = z(r,c) - zp(r,c);
        }
    }
    
    for (int c=0; c<v.cols(); c++) {
        v(1,c) = pi_to_pi(v(1,c));
    }

    float w = 1.0;
    
    MatrixXf S;
    float den, num;
    VectorXf vcol(v.rows()); 
    for (int i=0; i<z.cols(); i++) {
        S = Sf[i];
        den = 2*pi*sqrt(S.determinant());
        for (int r=0; r< v.rows(); r++) {
            vcol[r] = v(r,i);
        }
        cout<<"INSIDE COMPUTE_WEIGHT"<<endl;
        cout<<(-0.5 * vcol.transpose() * S.inverse() * vcol)<<endl;
        num = exp(-0.5 * vcol.transpose() * S.inverse() * vcol);
        w = w* num/den; 
    }
    return w;
} 
