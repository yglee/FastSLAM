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
    MatrixXf X;
	X = nRandMat::randn(len,n);	
    	
	//VectorXf ones = //VectorXf::Ones(n).transpose();
	MatrixXf ones = MatrixXf::Ones(1,n);	
	return S*X + x*ones;
}

