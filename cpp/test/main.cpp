#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <stdio.h>
#include <iostream>
#include "particle.h"
#include <vector>

using namespace std;
using namespace Eigen;

void test(Particle **a) 
{
	Particle *particles2 = new Particle[4];
	for (int i =0; i< 4; i++) {
		particles2[i].x = 1;
	}
	*a = particles2;
}


int main (int argc, char*argv[]) 
{

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
}
