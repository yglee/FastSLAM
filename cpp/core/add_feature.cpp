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
    int lenz = z.size();//z.cols();
    //MatrixXf xf(2,lenz); //measurements (range, bearing)
    vector<VectorXf> xf;
	vector<MatrixXf> Pf;
    VectorXf xv = particle.xv();

    float r,b,s,c;
    MatrixXf Gz(2,2);

    for (int i=0; i<lenz; i++) {
        r = z[i][0];//z(0,i);
        b = z[i][1];//z(1,i);
        s = sin(xv(2)+b);
        c = cos(xv(2)+b);
		
		VectorXf measurement(2);
		measurement(0) = xv(0) + r*c;
		measurement(1) = xv(1) + r*s;
 		xf.push_back(measurement);
        //xf(0,i) = xv(0) + r*c;
        //xf(1,i) = xv(1) + r*s;
        Gz <<c,-r*s,s,r*c;

        Pf.push_back(Gz*R*Gz.transpose()); 	
    }

    //int lenx = particle.xf().cols();
    int lenx = particle.xf().size();
    vector<int> ii;
    for (int i=lenx; i<lenx+lenz; i++) {
        ii.push_back(i);
    }	

	/*
    //max value in ii
    vector<int>::iterator maxelem;
    //int pcols = particle.xf().cols();
    int pcols = particle.xf().size();
    maxelem = std::max_element(ii.begin(),ii.end());
    int xf_size = std::max((*maxelem)+1,pcols);
	particle.xf().resize(xf_size);
	*/

	for(int j=0; j<ii.size(); j++) {
		particle.setXfi(ii[j],xf[j]);
	}
	
/*
    MatrixXf xfCopy(prows,xfc);
    xfCopy.setZero();
    for (int i =0; i<prows; i++) {
        for (int j=0; j<pcols; j++) {
            xfCopy(i,j) =particle.xf()(i,j); 
        }
    }
    
*/

    //stupid matlab... I need to dynamically
    //pfCopy
/*
    int pfcols = particle.Pf().size();
    int Pfc = std::max((*maxelem)+1,pfcols); 
	particle.Pf().resize(Pfc);
*/	
	for(int i=0; i<ii.size(); i++) {
		particle.setPfi(ii[i],Pf[i]);
	}
#if 0 
    vector<MatrixXf> pfCopy(Pfc);
    for (int i=0; i<pfcols; i++) {
        pfCopy[i] = particle.Pf()[i];
    }

    for (unsigned l=0; l<ii.size(); l++) {
        if (ii.size() == 0) {
            break;
        }
        if (xfCopy.isZero()) {
            xfCopy = xf;
        } else {			
            for (unsigned k=0; k<xf.rows(); k++) {
                xfCopy(k,ii[l]) = xf(k,l); 
            }
        }	
        if (pfCopy.empty()) {
            pfCopy = Pf;
        } else {	
            pfCopy[ii[l]] = Pf[l];	
        }
    }

    particle.setXf(xfCopy);
    particle.setPf(pfCopy);
#endif
}
