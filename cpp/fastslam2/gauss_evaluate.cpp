#include "gauss_evaluate.h"
#include <algorithm>
#include <iostream>

#define pi 3.14159

using namespace std; 

//approximation of the desired sampling distribution. 
float gauss_evaluate(VectorXf v, MatrixXf S, int logflag) 
{
    int D = v.size();    
    MatrixXf Sc = S.llt().matrixL();

    //normalised innovation
    VectorXf ni = Sc.jacobiSvd(ComputeThinU | ComputeThinV).solve(v);

    float E=0.f;
    VectorXf nin(ni.size());
    for (int s=0; s< ni.size(); s++) {
        nin[s] = ni[s]*ni[s];
    }
    E = -0.5f * nin.sum();

    int i,j;
    float C,w;
    unsigned  m = min(Sc.rows(), Sc.cols());

    if (logflag !=1) {
        float prod = 1;
        for (i=0; i<Sc.rows(); i++) {
            for (j=0; j<Sc.cols(); j++) {
                if (i==j) {
                    prod=prod*Sc(i,j); //multiply the diagonals 
                }
            }
        }
        C = pow((2*pi),(D/2))*prod; //normalizing term(makes Gaussian hyper volume =1
        w = exp(E)/C; //likelihood (exp(E) is targe distribution).
        //C is the proposal distribution
    } else {
        float sum=0;
        for (i=0; i<m; i++) {
            sum+=log(Sc(i,i)); 
        }
        C = 0.5*D*log(2*pi) + sum; //log of normalising term
        w = E-C; //log-likelihood
    }    
    return w;
}
