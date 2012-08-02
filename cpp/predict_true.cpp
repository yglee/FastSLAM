#include "predict_true.h"
#include "pi_to_pi.h"

void predict_true(VectorXf &xv, float V, float G, int WB, float dt) 
{
	xv(0) = xv(0) + V*dt*cos(G+xv(2));		
	xv(1) = xv(1) + V*dt*sin(G+xv(2));
	xv(2) = pi_to_pi(xv(2) + V*dt*sin(G)/WB);		
}
