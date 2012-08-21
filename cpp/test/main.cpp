#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "particle.h"

using namespace std;
using namespace Eigen;

#include <Eigen/SVD>

#if 0
void test(Particle **a) 
{
	Particle *particles2 = new Particle[4];
	for (int i =0; i< 4; i++) {
		particles2[i].x = 1;
	}
	*a = particles2;
}
#endif

int main (int argc, char*argv[]) 
{

#if 0
	Particle *particles = new Particle[10];

	for (int j=0;j< 10; j++) {
		particles[j].x = 2;
	}

	for (int k=0; k<20; k++) {
		cout<<particles[k].x<<" ";
	}
	cout<<" "<<endl;	
		
	test(&particles);

	for (int m=0; m<10; m++) {
		cout<<particles[m].x<<" ";
	}	
	
	cout<<" "<<endl;	

	vector<Particle> newPart(3);
	newPart[2].x = 300;
	cout<<newPart[0].x<<" " <<newPart[1].x<<endl;

	vector<Particle> oldPart = vector<Particle>(newPart);
	newPart[2].x = 1000;
	oldPart[2].x = 500;
	cout<<oldPart[2].x<<endl;
	cout<<newPart[2].x<<endl;
#endif

	MatrixXf test(3,3);
	test<<5.59645e-05, -3.26374e-06, -8.15936e-07,
				-3.26374e-06,  1.89453e-05,  4.73632e-06,
				-8.15936e-07,  4.73632e-06,  1.18408e-06;

	test<<5.5964464991120621562e-05, -3.2637426556902937591e-06, -8.1593566392257343978e-07,
-3.2637424283166183159e-06, 1.8945262127090245485e-05, 4.7363155317725613713e-06,
-8.1593560707915457897e-07, 4.7363155317725613713e-06, 1.1840788829431403428e-06;

	MatrixXf Pv(test);

	cout<<"Pv"<<endl;
	cout<<Pv<<endl;
	cout<<"Pv.inverse()"<<endl;
	cout<<Pv.inverse()<<endl;

        MatrixXf A_inv = Pv.llt().solve(MatrixXf::Identity(Pv.rows(),Pv.cols()));
        cout<<"A_inv"<<endl;
        cout<<A_inv<<endl;  
  
}
