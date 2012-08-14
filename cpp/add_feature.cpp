#include "add_feature.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

//
// add new features
//
void add_feature(Particle &particle, MatrixXf z, MatrixXf R)
{	
    int lenz = z.cols();
    MatrixXf xf(2,lenz);
    vector<MatrixXf> Pf;
	VectorXf xv = particle.xv();
	 			
	float r,b,s,c;
	MatrixXf Gz(2,2);

	for (int i=0; i<lenz; i++) {
		r = z(0,i);
		b = z(1,i);
		s = sin(xv(2)+b);
		c = cos(xv(2)+b);

		xf(0,i) = xv(0) + r*c;
		xf(1,i) = xv(1) + r*s;
		Gz <<c,-r*s,s,r*c;
	
		cout<<"Gz"<<endl;
		cout<<Gz<<endl;
		cout<<"R"<<endl;	
		cout<<R<<endl;	

		Pf.push_back(Gz*R*Gz.transpose()); 	
		cout<<"Pf[0]"<<endl;
		cout<<Pf[0]<<endl;
	}

	int lenx = particle.xf().cols();
	vector<int> ii;
	for (int i=lenx; i<lenx+lenz; i++) {
		ii.push_back(i);
		cout<<"i = "<<i<<endl;
	}	

	//copy out the xf and Pv matrices so I can modify it.
	MatrixXf xfcopy = particle.xf();	
	//TODO: WARNING: xfcopy is empty at this point
	vector<MatrixXf> pfcopy(particle.Pf());  

	for (unsigned c=0; c<ii.size(); c++) {		
		for (unsigned r=0; r<xf.rows(); r++) {	
			cout<<"xfcopy(r:"<<r<<" ii["<<c<<"]:"<<ii[c]<<")"<<endl;
			
			xfcopy(r,ii[c]) = xf(r,c); 
		}	
		if (pfcopy.empty()) {
			pfcopy(Pf);
		} else {	
			pfcopy[ii[c]] = Pf[c];	
		}
	}
	
	particle.setXf(xfcopy);
	particle.setPf(pfcopy);
}
