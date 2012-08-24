#ifndef KF_JOSEPH_UPDATE_H
#define KF_JOSEPH_UPDATE_H

#include <Eigen/Dense>

using namespace Eigen;

void KF_joseph_update(VectorXf &x,MatrixXf &P,float v,float R, MatrixXf H);
MatrixXf make_symmetric(MatrixXf P);

#endif
