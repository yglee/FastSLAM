#include "multivariate_gauss.h"
#include "add_observation_noise.h"
#include <Eigen/Cholesky>
#include <iostream>

using namespace std;
//x is mean vector
//P is covariance matrix
//n obtain n samples

//output: sample set

VectorXf multivariate_gauss(VectorXf x, MatrixXf P, int n) 
{
	int len = x.size();
	//choleksy decomposition
	MatrixXf S = P.llt().matrixL();
	MatrixXf X(len,n);
	

    float LO = -1.0f;
    float HI = 1.0f;

    for (int i = 0; i < len; i++) {
        for (int j=0; j< n; j++) {
            float r3 = LO + (float)rand()/((float)RAND_MAX/(HI-LO));
            X(i,j) = r3;
        }
    }

    //TODO: this does not work. Also fixed other instances of nRandMat	
	//X = nRandMat::randn(len,n);	
	
	MatrixXf ones = MatrixXf::Ones(1,n);	
	return S*X + x*ones;
}

