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
    cout<<"ADD_FEATURE"<<endl;
    cout<<"z"<<endl;
    cout<<z<<endl;
    cout<<"R"<<endl;
    cout<<R<<endl;
    cout<<"particle.w"<<endl;
    cout<<particle.w()<<endl;
    cout<<"particle.xv"<<endl;
    cout<<particle.xv()<<endl;
    cout<<"particle.Pv"<<endl;
    cout<<particle.Pv()<<endl;
    cout<<"particle.xf"<<endl;
    cout<<particle.xf()<<endl;
    cout<<"particle.Pf"<<endl;
    for (int i=0; i<(particle.Pf()).size();i++) {
        cout<<particle.Pf()[i]<<endl;
    }
	
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

        Pf.push_back(Gz*R*Gz.transpose()); 	
    }

    int lenx = particle.xf().cols();
    vector<int> ii;
    for (int i=lenx; i<lenx+lenz; i++) {
        ii.push_back(i);
    }	

    //MatrixXf xfCopy;// = particle.xf();	
    //vector<MatrixXf> pfCopy(particle.Pf());  

    cout<<"xf"<<endl;
    cout<<xf<<endl;

//TODO: there is a bug here. xfCopy (which is particle.xf) should grow in size 
//based on length of ii
//new xfCopy columns = max(ii.size(),xfCopy.cols())
//initialize to 0

    
    //stupid dynamically sized matlab matrices :((((
    int prows = particle.xf().rows();
    int pcols = particle.xf().cols();
    int iisize = ii.size();
 
    //max value in ii
    vector<int>::iterator maxelem;
    maxelem = std::max_element(ii.begin(),ii.end());
    int xfc = std::max((*maxelem)+1,pcols);

    cout<<"particle.xf().cols()"<<endl;
    cout<<pcols<<endl;
    cout<<"max"<<endl;
    cout<<xfc<<endl;
    MatrixXf xfCopy(prows,xfc);
    xfCopy.setZero();
    for (int i =0; i<prows; i++) {
        for (int j=0; j<pcols; j++) {
            xfCopy(i,j) =particle.xf()(i,j); 
        }
    }
    

    //stupid matlab... I need to dynamically
    //pfCopy
    int pfcols = particle.Pf().size();
    int Pfc = std::max((*maxelem)+1,pfcols); 
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
}
