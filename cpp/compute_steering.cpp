#include "compute_steering.h"

void compute_steering(Vector3d x, MatrixXd wp, int iwp, float minD, float G, 
					  float rateG, float max G, float dt)
{
/*
% INPUTS:
%   x - true position
%   wp - waypoints
%   iwp - index to current waypoint
%   minD - minimum distance to current waypoint before switching to next
%   G - current steering angle
%   rateG - max steering rate (rad/s)
%   maxG - max steering angle (rad)
%   dt - timestep
*/

	//determine if current waypoint reached
	Vector2d cwp;
	cwp[0] = wp[0][iwp];     
	cwp[1] = wp[1][iwp];
	
	d2 = (cwp[0] - x[0])^2 + (cwp[1]-x[1])^2;     
	
	if (d2 < minD*minD) {
		iwp = iwp+1; //switch to next
		if (iwp > wp.cols()) {
			iwp =0;
			return;	
		}
		cwp[0] = wp[0][iwp];
		cwp[1] = wp[1][iwp];
	}
	
	//compute change in G to point towards current waypoint
	float deltaG = pi_to_pi(atan2(cwp[1]-x[1], cwp[0]-x[0]) - x[2] - G);	

	//limit rate
	float maxDelta = rateG*dt;
	if (abs(deltaG) > maxDelta) {
		int sign = (deltaG > 0) ? 1 : ((deltaG < 0) ? -1 : 0);
		delatG = sign*maxDelta;	
	}	

	//limit angle
	G = G+deltaG;
	if (abs(G) > maxG) {
		int sign2 = (G > 0) ? 1: ((G < 0) ? -1 : 0);
		G = sign2*maxG;
	}	
}
