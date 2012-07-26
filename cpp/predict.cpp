#include "predict.h"
#include <math.h>
#include <iostream>

#define pi 3.14159265

using namespace std;

void predict(Particle &particle,float V,float G,MatrixXd Q, float WB,float dt, int addrandom)
{	
	Vector3f xv = particle.xv();
	Matrix3f Pv = particle.Pv();

	
	//Jacobians
	float phi = xv(3);
	MatrixXf Gv(3,3); 
	Gv << 1,0,-V*dt*sin(G+phi),
		0,1,V*dt*cos(G+phi),
		0,0,1;
	MatrixXf Gu(3,2); 
	Gu << dt*cos(G+phi), -V*dt*sin(G+phi),
		dt*sin(G+phi), V*dt*cos(G+phi),
		dt*sin(G)/WB, V*dt*cos(G)/WB;
	
	//predict covariance
	cout<<"Gv"<<endl;
	cout<<Gv<<endl;
	cout<<"Pv"<<endl;
	cout<<Pv<<endl;
	cout<<"Gv*Pv*Gv.tranpose()"<<endl;
	cout<<Gv*Pv*Gv.transpose()<<endl;
	//MatrixXf newPv = Gv*Pv*Gv.transpose() + Gu*Q*Gu.transpose();
	//particle.setPv(newPv);		

	/*
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
	xv_temp(0) = xv(0) + V*dt*cos(G+xv(2));	
	xv_temp(1) = xv(1) + V*dt*sin(G+xv(2));	
	xv_temp(2) = pi_to_pi(xv(2) + V*dt*sin(G/WB));				
	particle.setXv(xv_temp);
		*/
}

float pi_to_pi(float x) 
{
	if (x > pi) {
		x = x-2*pi;
	} else if (x < -pi) {
		x = x+2*pi;
	}
	return x;
}

