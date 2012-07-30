#include "KF_joseph_update.h"

//TODO: check this!
void KF_joseph_update(Vector3f &x, Matrix3f &P,float v,float R, MatrixXf H)
{
    Vector3f PHt = P*H.transpose();
    
	MatrixXf S = H*PHt;
	S(0,0) += R;
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
}

MatrixXf make_symmetric(MatrixXf P)
{
    return (P + P.transpose())*0.5;
}
