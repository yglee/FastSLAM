#include "add_control_noise.h"
#include <iostream>

using namespace std;

void add_control_noise(float &V, float &G, Matrix2f Q, int addnoise, float* VnGn) 
{
	if (addnoise ==1) {
		VectorXf A(2);
		A(0) = V;
		A(1) = G;
		VectorXf C(2);
		C = multivariate_gauss(A,Q,1);
		VnGn[0] = C(0);
		VnGn[1] = C(1);
	}
}
