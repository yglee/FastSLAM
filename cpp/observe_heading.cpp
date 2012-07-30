#include "observe_heading.h"
#include "pi_to_pi.h"
#include "KF_joseph_update.h"

#include <iostream>
#include <vector>

using namespace std;

void observe_heading(Particle &particle, float phi, int useheading) 
{
    if (useheading ==0)
            return;
    float sigmaPhi = 0.01*pi/180.0; 	
    Vector3f xv = particle.xv();		
    Matrix3f Pv = particle.Pv();
    
    MatrixXf H(1,3);
    H<<0,0,1;            

    std::vector<float> vecV;
    vecV.push_back(phi-xv(2));
    pi_to_pi(vecV);	
    float v = vecV[0]; 
    
    KF_joseph_update(xv,Pv,v,pow(sigmaPhi,2),H);
    particle.setXv(xv);
    particle.setPv(Pv);
}
