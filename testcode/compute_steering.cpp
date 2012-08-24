#include "compute_steering.h"
#include "pi_to_pi.h"

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

void compute_steering(VectorXf x, MatrixXf wp, int& iwp, float minD, 
				float& G, float rateG, float maxG, float dt)
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
		cwp[0] = wp(0,iwp); //-1 since indexed from 0     
		cwp[1] = wp(1,iwp);

		float d2 = pow((cwp[0] - x[0]),2) + pow((cwp[1]-x[1]),2);     

		if (d2 < minD*minD) {
				iwp++; //switch to next
				if (iwp >= wp.cols()) {
				    iwp =-1;
				    return;	
				}

				cwp[0] = wp(0,iwp); //-1 since indexed from 0
				cwp[1] = wp(1,iwp);
		}

		//compute change in G to point towards current waypoint
		float deltaG = atan2(cwp[1]-x[1], cwp[0]-x[0]) - x[2] - G;
		deltaG = pi_to_pi(deltaG);

		//limit rate
		float maxDelta = rateG*dt;
		if (abs(deltaG) > maxDelta) {
				int sign = (deltaG > 0) ? 1 : ((deltaG < 0) ? -1 : 0);
				deltaG = sign*maxDelta;	
		}	

		//limit angle
		G = G+deltaG;
		if (abs(G) > maxG) {
				int sign2 = (G > 0) ? 1: ((G < 0) ? -1 : 0);
				G = sign2*maxG;
		}
}
