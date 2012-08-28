#include "KF_cholesky_update.h"

void KF_cholesky_update(VectorXf &x, MatrixXf &P,VectorXf v,MatrixXf R,MatrixXf H)
{
    MatrixXf PHt = P*H.transpose();
    MatrixXf S = H*PHt + R;
    
    S = (S+S.transpose()) * 0.5; //make symmetric
    MatrixXf SChol = S.llt().matrixL();
    SChol.transpose();
    SChol.conjugate();

    MatrixXf SCholInv = SChol.inverse(); //tri matrix
    MatrixXf W1 = PHt * SCholInv;
    MatrixXf W = W1 * SCholInv.transpose();

    x = x + W*v;
    P = P - W1*W1.transpose();
}
