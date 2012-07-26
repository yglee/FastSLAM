#ifndef KF_JOSEPH_UPDATE_H
#define KF_JOSEPH_UPDATE_H

#include <Eigen/Dense>

void KF_joseph_update(Vector3f x,Matrix3f P,float v,float R, Vector3f H);

#endif
