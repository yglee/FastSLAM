#include "add_control_noise.h"
#include <iostream>

using namespace std;

void add_control_noise(float &V, float &G, Matrix2f Q, int addnoise) 
{
	if (addnoise ==1) {
		VectorXf A(2);
		A(0) = V;
		A(1) = G;
		//TODO: fix this line
		VectorXf C(2);
		C = multivariate_gauss(A,Q,1);
		V = C(0);
		G = C(1);
	}
}
