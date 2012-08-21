#include "predict.h"
#include <math.h>
#include <iostream>

//checked all values. everything works

using namespace std;

void predict(Particle &particle,float V,float G,Matrix2f Q, float WB,float dt, int addrandom)
{
	VectorXf xv = particle.xv();
        MatrixXf Pv = particle.Pv();

	//Jacobians
	float phi = xv(2);
	MatrixXf Gv(3,3); 

	Gv << 1,0,-V*dt*sin(G+phi),
	       0,1,V*dt*cos(G+phi),
	       0,0,1;
	MatrixXf Gu(3,2); 
	Gu << dt*cos(G+phi), -V*dt*sin(G+phi),
		dt*sin(G+phi), V*dt*cos(G+phi),
		dt*sin(G)/WB, V*dt*cos(G)/WB;

	//predict covariance
	MatrixXf newPv;//(Pv.rows(),Pv.cols());
        //TODO: Pv here is somehow corrupted. Probably in sample_proposal
        cout<<"Pv in predict"<<endl;
        cout<<Pv<<endl;
        newPv = Gv*Pv*Gv.transpose() + Gu*Q*Gu.transpose();
        particle.setPv(newPv);	

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
	VectorXf xv_temp(3);
        xv_temp << xv(0) + V*dt*cos(G+xv(2)),
	           xv(1) + V*dt*sin(G+xv(2)),
	           pi_to_pi(xv(2) + V*dt*sin(G/WB));
        particle.setXv(xv_temp);
}
