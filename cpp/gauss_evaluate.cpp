#include "gauss_evaluate.h"
#include <algorithm>
#define pi 3.14159

using namespace std; 

VectorXf gauss_evaluate(VectorXf v, MatrixXf S, int logflag) 
{
    int D = v.size();    
    MatrixXf Sc = S.llt().matrixL();
    //normalised innovations
    MatrixXf nin = Sc.jacobiSvd(ComputeThinU | ComputeThinV).solve(v);
    
    int r,c;
    VectorXf E(nin.cols());
    for (c=0; c<nin.cols(); c++) {
        for (r=0; r<nin.rows(); r++) {
            nin(r,c) = pow(nin(r,c),2); 
            E(c) += nin(r,c);
        }
        E(c) = -0.5*E(c); //Gaussian exponential term
    }
    //Note: writing sum (x.*x,1) is a fast way to compute sets of inner-products
    
    VectorXf w(E.size());
    float C,i,j;
    int m = min(Sc.rows(), Sc.cols());
    if (logflag !=1) {
        float prod = 1;
        for (i=0; i<m; i++) {
            prod=prod*Sc(i,i); //multiply the diagonals 
        }
        C = pow((2*pi),(D/2))*prod; //normalizing term(makes Gaussian hyper volume =1) 
        for (j=0; j<E.size(); j++) {
            w(j) = exp(E(j))/C; //likelihood
        }        
    } else {
        float sum=0;
        for (i=0; i<m; i++) {
            sum+=log(Sc(i,i)); 
        }
        C = 0.5*D*log(2*pi) + sum; //log of normalising term
        for (j=0; j<E.size(); j++) {
            w(j) = E(j)-C; //log-likelihood
        }
    }    
    return w;
}
