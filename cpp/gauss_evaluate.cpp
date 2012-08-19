#include "gauss_evaluate.h"
#include <algorithm>
#include <iostream>

#define pi 3.14159

using namespace std; 


float gauss_evaluate(VectorXf v, MatrixXf S, int logflag) 
{
    #if 0
    cout<<"in gauss_eval"<<endl;
    cout<<"v"<<endl;
    cout<<v<<endl;
    cout<<endl;
    cout<<"S"<<endl;
    cout<<S<<endl;
    #endif
    
    int D = v.size();    
    MatrixXf Sc = S.llt().matrixL();

    //normalised innovations
    VectorXf nin = Sc.jacobiSvd(ComputeThinU | ComputeThinV).solve(v);
    
    #if 0
    cout<<"nin"<<endl;
    cout<<nin<<endl;
    #endif

    int s;
    //VectorXf E(nin.cols());
    float E=0;
    for (s=0; s<nin.size(); s++) {
        nin(s) = pow(nin(s),2);
        E += nin(s);
    }
    E=-0.5*E;
    //Note: writing sum (x.*x,1) is a fast way to compute sets of inner-products

    unsigned i;
    float C,w;
    unsigned  m = min(Sc.rows(), Sc.cols());

    if (logflag !=1) {
        float prod = 1;
        for (i=0; i<m; i++) {
            prod=prod*Sc(i,i); //multiply the diagonals 
        }
        C = pow((2*pi),(D/2))*prod; //normalizing term(makes Gaussian hyper volume =1) 
        w = exp(E)/C; //likelihood
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
