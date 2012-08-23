#include "multivariate_gauss.h"
#include <Eigen/Cholesky>
#include <iostream>

using namespace std;

VectorXf multivariate_gauss(VectorXf x, MatrixXf P, int n) 
{
	int len = x.size();
	//choleksy decomposition
	MatrixXf S = P.llt().matrixL();
	cout<<"S"<<endl;
        cout<<S<<endl;
        cout<<endl;
        MatrixXf X(len,n); //TODO this needs to be randomized
        X<<-0.8809,0.8571,0.2638;
	VectorXf ones = VectorXf::Ones(n).transpose();
	return S*X + x*ones;
}

