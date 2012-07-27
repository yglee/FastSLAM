#include "KF_joseph_update.h"
#include <iostream>

using namespace std;

//TODO: check this!
void KF_joseph_update(Vector3f &x, Matrix3f &P,float v,float R, MatrixXf H)
{
    Vector3f PHt = P*H.transpose();
    //TODO: should S and Si be 1x1 matrices?
    

	cout<<"H"<<endl;
	cout<<H<<endl;
	cout<<"PHt"<<endl;
	cout<<PHt<<endl;
	cout<<"R"<<endl;
	cout<<R<<endl;
	#if 0
	MatrixXf S = H*PHt + R;
    MatrixXf Si = S.inverse();	
    Si = make_symmetric(Si); 
    MatrixXf PSD_check = Si.llt().matrixL(); //chol of scalar is sqrt
    PSD_check.transpose();
    PSD_check.conjugate();

    Vector3f W = PHt*Si;
    x = x+W*v;
    
    //Joseph-form covariance update
    MatrixXf eye(P.rows(), P.cols());
    eye.setIdentity();
    MatrixXf C = eye - W*H;
    P = C*P*C.transpose() + W*R*W.transpose();  
   
    float eps = 2.2204*pow(10,-16); //numerical safety 
    P = P+eye*eps;
    PSD_check = P.llt().matrixL();
    PSD_check.transpose();
    PSD_check.conjugate(); //for upper tri
	#endif
}

MatrixXf make_symmetric(MatrixXf P)
{
    return (P + P.transpose())*0.5;
}
