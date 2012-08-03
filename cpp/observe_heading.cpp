#include "observe_heading.h"
#include "pi_to_pi.h"
#include "KF_joseph_update.h"

#include <iostream>
#include <vector>

using namespace std;

void observe_heading(Particle &particle, float phi, int useheading) 
{
    if (useheading ==0){
    	return;
    }
	float sigmaPhi = 0.01*pi/180.0; 	
    VectorXf xv = particle.xv();		
    MatrixXf Pv = particle.Pv();
    
    MatrixXf H(1,3);
    H<<0,0,1;            

    float v = pi_to_pi(phi-xv(2));
    KF_joseph_update(xv,Pv,v,pow(sigmaPhi,2),H);
	
    particle.setXv(xv);
    particle.setPv(Pv);
}
