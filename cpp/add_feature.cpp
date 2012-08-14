#include "add_feature.h"
#include <math.h>
#include <vector>

using namespace std;

//
// add new features
//
void add_feature(Particle &particle, MatrixXf z, MatrixXf R)
{	
    int lenz = z.cols();
    MatrixXf xf(2,lenz);
    MatrixXf *Pf;
	VectorXf xv = particle.xv();
	 			
	float r,b,s,c;
	int row;
	MatrixXf Gz(2,2);
	
	for (int i=0; i<lenz; i++) {
		r = z(0,i);
		b = z(1,i);
		s = sin(xv(2)+b);
		c = cos(xv(2)+b);

		xf(0,i) = xv(0) + r*c;
		xf(1,i) = xv(1) + r*s;
		Gz << c, -r*s, s, r*c;
		Pf[i] = Gz*R*Gz.transpose(); //TODO: double check	
	}

	int lenx = particle.xf().size();
	
	vector<int> ii;
	for (int i=lenx; i<lenx+lenz; i++) {
		ii.push_back(i);
	}	

	//copy out the xf matrix
	MatrixXf xfcopy = particle.xf();		
	MatrixXf *pfcopy = particle.Pf();

	for (int c=0; c<ii.size(); c++) {		
		for (int r=0; r<xf.rows(); r++) {	
			xfcopy(r,ii[c]) = xf(r,c); //TODO: double check
		}	
		pfcopy[ii[c]] = Pf[c];//TODO: not sure..	
	}
	
	particle.setXf(xfcopy);
	particle.setPf(pfcopy);
}
