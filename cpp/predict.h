#ifndef PREDICT_H
#define PREDICT_H

#include <Eigen/Dense>
#include "particles.h"

using namespace Eigen;

void predict(Particle &particle,float V,float G,MatrixXd Q, float WB,float dt, int addrandom);

float pi_to_pi(float x);
#endif //PREDICT_H
