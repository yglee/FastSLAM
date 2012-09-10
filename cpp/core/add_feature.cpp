#include "add_feature.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

//
// add new features
//
void add_feature(Particle &particle, vector<VectorXf> z, MatrixXf R)
{
    int lenz = z.size();
    vector<VectorXf> xf;
    vector<MatrixXf> Pf;
    VectorXf xv = particle.xv();

    float r,b,s,c;
    MatrixXf Gz(2,2);

    for (int i=0; i<lenz; i++) {
	r = z[i][0];
	b = z[i][1];
	s = sin(xv(2)+b);
	c = cos(xv(2)+b);

	VectorXf measurement(2);
	measurement(0) = xv(0) + r*c;
	measurement(1) = xv(1) + r*s;
	xf.push_back(measurement);
	Gz <<c,-r*s,s,r*c;

	Pf.push_back(Gz*R*Gz.transpose()); 	
    }

    int lenx = particle.xf().size();
    vector<int> ii;
    for (int i=lenx; i<lenx+lenz; i++) {
	ii.push_back(i);
    }	

    for(int j=0; j<ii.size(); j++) {
	particle.setXfi(ii[j],xf[j]);
    }


    for(int i=0; i<ii.size(); i++) {
	particle.setPfi(ii[i],Pf[i]);
    }
}
