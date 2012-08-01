#include "compute_jacobians.h"
#include "math.h"

void compute_jacobians(Particle particle, vector<int> idf, MatrixXf R)
{
	VectorXf xv = particle.xv();
	MatrixXf Pv = particle.Pv();
	VectorXf xv0 = VectorXf(xv);
	MatrixXf Pv0 = MatrixXf(Pv);	

	float dx,dy,d2,d;

	for (int i=0; i<idf.size(); i++) {
		dx = xf(1,i) - xv(1);
		dy = xf(2,i) - xv(2);
		d2 = pow(dx,2) + pow(dy,2);	
		d = sqrt(d2);
	
	}			
}
