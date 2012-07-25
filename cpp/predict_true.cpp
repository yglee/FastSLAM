#include "predict_true.h"

void predict_true(VectorXf &xv, float V, float G, int WB, float dt) 
{
	xv(0) = xv(0) + V*dt*cos(G+xv(2));		
	xv(1) = xv(1) + V*dt*sin(G+xv(2));
	vector<float> angle;
	angle.push_back(xv(2) + V*dt*sin(G)/WB); //pi_to_pi takes a vector.		
	pi_to_pi(angle);		
	xv(2) = angle[0];		
}
