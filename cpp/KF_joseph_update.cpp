#include "KF_joseph_update.h"
#include <iostream>
#include <math.h>

using namespace std;

//TODO: check this!
void KF_joseph_update(VectorXf &x, MatrixXf &P,float v,float R, MatrixXf H)
{
    VectorXf PHt = P*H.transpose();
    cout<<"KF_Joseph: PHt"<<endl;
    cout<<PHt<<endl; 
    MatrixXf S = H*PHt;
    S(0,0) += R;
    cout<<"S"<<endl;
    cout<<S<<endl;
    MatrixXf Si = S.inverse();
    cout<<"Si before symmetric"<<endl;
    cout<<Si<<endl;	
    Si = make_symmetric(Si);
    cout<<"Si after symmetric"<<endl;
    cout<<Si<<endl; 
    MatrixXf PSD_check = Si.llt().matrixL(); //chol of scalar is sqrt
    PSD_check.transpose();
    PSD_check.conjugate();
    cout<<"PSD_check"<<endl;
    cout<<PSD_check<<endl;

    VectorXf W = PHt*Si;
    cout<<"W"<<endl;
    cout<<W<<endl;
    x = x+W*v;
    cout<<"x"<<endl;
    cout<<x<<endl;
    
    //Joseph-form covariance update
    MatrixXf eye(P.rows(), P.cols());
    eye.setIdentity();
    MatrixXf C = eye - W*H;
    cout<<"C"<<endl;
    cout<<C<<endl;
    P = C*P*C.transpose() + W*R*W.transpose();  
    cout<<"P"<<endl;
    cout<<P<<endl;   

    float eps = 2.2204*pow(10.0,-16); //numerical safety 
    cout<<"eps"<<endl;
    cout<<eps<<endl;
    P = P+eye*eps;
    cout<<"P"<<endl;
    cout<<P<<endl;	

    PSD_check = P.llt().matrixL();
    PSD_check.transpose();
    PSD_check.conjugate(); //for upper tri
    //break
}

MatrixXf make_symmetric(MatrixXf P)
{
    return (P + P.transpose())*0.5;
}
