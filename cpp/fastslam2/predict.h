#ifndef PREDICT_H
#define PREDICT_H

#include <Eigen/Dense>
#include "core/particle.h"
#include "core/multivariate_gauss.h"

using namespace Eigen;

void predict(Particle &particle,float V,float G,Matrix2f Q, float WB,float dt, int addrandom);

float pi_to_pi2(float ang); 

#endif //PREDICT_H
