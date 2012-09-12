#include "observe_heading.h"
#include "core/pi_to_pi.h"
#include "core/KF_joseph_update.h"

#include <iostream>
#include <vector>

//observe_heading doesn't get called unless I change SWITCH_HEADING KNOWN

using namespace std;

void observe_heading(Particle &particle, float phi, int useheading) 
{
    if (useheading ==0){
    	return;
    }
    float sigmaPhi = 0.01*pi/180.0; 
    //cout<<"sigmaPhi "<<sigmaPhi<<endl;	
    VectorXf xv = particle.xv();
    //cout<<"xv"<<endl;
    //cout<<xv<<endl;		
    MatrixXf Pv = particle.Pv();
    //cout<<"Pv"<<endl;
    //cout<<Pv<<endl;    
    MatrixXf H(1,3);
    H<<0,0,1;            

    float v = pi_to_pi(phi-xv(2));
    //cout<<"v"<<endl;
    //cout<<v<<endl;
    KF_joseph_update(xv,Pv,v,pow(sigmaPhi,2),H);
    //cout<<"KF_Joseph = xv"<<endl;
    //cout<<xv<<endl;
    //cout<<"KF_Joseph = Pv"<<endl;
    //cout<<Pv<<endl;

    particle.setXv(xv);
    particle.setPv(Pv);
}
