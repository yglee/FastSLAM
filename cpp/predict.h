#ifndef PREDICT_H
#define PREDICT_H

#include <Eigen/Dense>
#include "particle.h"
#include "multivariate_gauss.h"
#include "pi_to_pi.h"

using namespace Eigen;

void predict(Particle &particle,float V,float G,Matrix2f Q, float WB,float dt, int addrandom);

#endif //PREDICT_H
