#include "multivariate_gauss.h"
#include <Eigen/Cholesky>
#include <iostream>

using namespace std;

VectorXf multivariate_gauss(VectorXf x, MatrixXf P, int n) 
{
	int len = x.size();
	//choleksy decomposition
	MatrixXf S = P.llt().matrixL();
	//TODO: why is random doesn't change values?
	//MatrixXf X = MatrixXf::Random(len,n);
	MatrixXf X = MatrixXf::Ones(len,n);
	VectorXf ones = VectorXf::Ones(n).transpose();
	return S*X + x*ones;
}

