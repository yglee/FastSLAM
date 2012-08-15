#include <iostream>
#include "compute_jacobians.h"
#include "math.h"
#include "pi_to_pi.h"

void compute_jacobians(Particle particle, 
        vector<int> idf, 
        MatrixXf R,
        MatrixXf &zp, 
        vector<MatrixXf> *Hv, 
        vector<MatrixXf> *Hf, 
        vector<MatrixXf> *Sf)
{
    VectorXf xv = particle.xv();

    cout<<"particle.xf()"<<endl;
    cout<<particle.xf()<<endl;

    int rows = (particle.xf()).rows();
    MatrixXf xf(rows,idf.size());    
    vector<MatrixXf> Pf;
    int r;
    for (unsigned i=0; i<idf.size(); i++) {
        for (r=0; r<(particle.xf()).rows(); r++) {
            xf(r,i) = (particle.xf())(r,(idf[i]));	
        }
        if (particle.Pf().size() != 0) {
            Pf.push_back((particle.Pf())[idf[i]]); //particle.Pf is a array of matrices
        }
    }

    float dx,dy,d2,d;
    unsigned int i;
    MatrixXf HvMat(2,3);
    MatrixXf HfMat (2,2);

    cout<<"before for loop in compute_jacobians"<<endl;
    cout<<"idf.size()"<<idf.size()<<endl;
    cout<<"xv "<<endl;
    cout<<xv<<endl;
    cout<<"should be 1.5052"<<endl;
    cout<<"         -0.0293"<<endl;
    cout<<"         -0.0079"<<endl;
    cout<<endl;
    cout<<"xf"<<endl;
    cout<<xf<<endl;
    cout<<"should be 3.4011"<<endl;
    cout<<"        -25.6172"<<endl;	
    cout<<endl;
    cout<<"Pf"<<endl;
    cout<<Pf[0]<<endl;
    cout<<"should be 0.2005 0.0195"<<endl;
    cout<<"          0.0196 0.0120"<<endl;
    cout<<endl;

    for (i=0; i<idf.size(); i++) {
        dx = xf(0,i) - xv(0);
        dy = xf(1,i) - xv(1);
        d2 = pow(dx,2) + pow(dy,2);	
        d = sqrt(d2);

        cout<<"dx"<<dx<<endl;
        cout<<"dy"<<dy<<endl;
        cout<<"d2"<<d2<<endl;
        cout<<"d"<<d<<endl;
        cout<<"about to print zp"<<endl;
        //predicted observation
        zp(0,i) = d;
        cout<<"zp(0,i) is "<<zp(0,i)<<endl;
        zp(1,i) = atan2(dy,dx) - xv(2);

        cout<<"zp(1,i)"<<zp(1,i)<<endl;

        zp(1,i) = pi_to_pi(zp(1,i));

        //Jacobian wrt vehicle states
        HvMat<< -dx/d, -dy/d,  0, 
            dy/d2, -dx/d2,-1;

        //Jacobian wrt feature states
        HfMat<< dx/d,  dy/d,
            -dy/d2, dx/d2;
        Hv->push_back(HvMat);
        Hf->push_back(HfMat);

        //innovation covariance of 'feature observation given the vehicle'
        MatrixXf SfMat = HfMat*Pf[i]*HfMat.transpose() + R;
        Sf->push_back(SfMat);  
    }			
}
