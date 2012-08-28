#include "predict.h"
#include <math.h>
#include <iostream>

#define pi 3.1416

using namespace std;

void predict(Particle &particle,float V,float G,Matrix2f Q, float WB,float dt, int addrandom)
{
	//optional: add random noise to predicted state
	if (addrandom ==1) {
		VectorXf A(2);
		A(0) = V;
		A(1) = G;
		VectorXf VG(2);
		VG = multivariate_gauss(A,Q,1);	
		V = VG(0);
		G = VG(1);	
	}	

	//predict state
	VectorXf xv = particle.xv();
	VectorXf xv_temp(3);
	xv_temp << xv(0) + V*dt*cos(G+xv(2)),
			xv(1) + V*dt*sin(G+xv(2)),
			pi_to_pi2(xv(2) + V*dt*sin(G/WB));
	particle.setXv(xv_temp);
}

float pi_to_pi2(float ang) 
{
	if (ang > pi) {
		ang = ang - (2*pi);
	}
	if (ang < -pi) {
		ang = ang + (2*pi);
	}
	return ang;
}

