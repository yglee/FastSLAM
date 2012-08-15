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

    //TODO: this doesn't match :(
    cout<<"xv in add_feature"<<endl;
    cout<<xv<<endl;
    cout<<"should be"<<endl;
    cout<<"0.7648"<<endl;
    cout<<"0.0221"<<endl;
    cout<<"0.0050"<<endl;
    cout<<endl;

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

        Pf.push_back(Gz*R*Gz.transpose()); 	
    }

    int lenx = particle.xf().cols();
    vector<int> ii;
    for (int i=lenx; i<lenx+lenz; i++) {
        ii.push_back(i);
    }	

    MatrixXf xfCopy = particle.xf();	
    vector<MatrixXf> pfCopy(particle.Pf());  


    for (unsigned c=0; c<ii.size(); c++) {
        cout<<"ii"<<endl;
        cout<<ii[c]<<endl;

    
        if (xfCopy.isZero()) {
            xfCopy = xf;
        } else {			
            for (unsigned r=0; r<xf.rows(); r++) {	
                xfCopy(r,ii[c]) = xf(r,c); 
            }
        }	
        if (pfCopy.empty()) {
            pfCopy = Pf;
        } else {	
            pfCopy[ii[c]] = Pf[c];	
        }
    }

    cout<<"xf in add_feature"<<endl;
    cout<<xf<<endl;
    cout<<"should be"<<endl;
    cout<<"3.4011   25.7814"<<endl;
    cout<<"-25.6172 3.6347"<<endl;
    cout<<endl; 

    cout<<"xfCopy"<<endl;
    cout<<xfCopy<<endl;
    cout<<endl;

    particle.setXf(xfCopy);
    particle.setPf(pfCopy);
}
