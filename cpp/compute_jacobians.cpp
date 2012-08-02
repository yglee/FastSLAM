#include "compute_jacobians.h"
#include "math.h"
#include "pi_to_pi.h"

void compute_jacobians(Particle particle, vector<int> idf, MatrixXf R, \
				MatrixXf &zp, MatrixXf *Hv, MatrixXf *Hf, MatrixXf *Sf)
{
		VectorXf xv = particle.xv();

		int rows = (particle.xf()).rows();
		MatrixXf xf(rows,idf.size());    
		MatrixXf *Pf;

		int r,c;
		for (c=0; c<idf.size(); c++) {
				Pf[c] = (particle.Pf())[idf[c]]; //returns a matrix pointer
				for (r=0; r<rows; r++) {
						xf(r,c) = particle.xf()(r,idf[c]);
				}
		}  

		float dx,dy,d2,d;
		unsigned int i;
		MatrixXf HvMat(2,3);
		MatrixXf HfMat (2,2);

		for (i=0; i<idf.size(); i++) {
				dx = xf(0,i) - xv(0);
				dy = xf(1,i) - xv(1);
				d2 = pow(dx,2) + pow(dy,2);	
				d = sqrt(d2);

				//predicted observation
				zp(0,i) = d;
				zp(1,i) = atan2(dy,dx) - xv(2);
				pi_to_pi(zp(1,i));

				//Jacobian wrt vehicle states
				HvMat<< -dx/d, -dy/d,  0, 
						dy/d2, -dx/d2,-1;
				//Jacobian wrt feature states
				HfMat<<  dx/d,  dy/d,
						-dy/d2, dx/d2;
				Hv[i] = MatrixXf(HvMat);
				Hf[i] = MatrixXf(HfMat);
				//innovation covariance of 'feature observation given the vehicle'
				MatrixXf SfMat = HfMat*Pf[i]*HfMat.transpose() + R;
				Sf[i] = MatrixXf(SfMat);  
		}			
}
