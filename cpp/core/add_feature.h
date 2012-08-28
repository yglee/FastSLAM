#ifndef ADD_FEATURE_H
#define ADD_FEATURE_H

#include <Eigen/Dense>

#include "particle.h"

using namespace Eigen;

void add_feature(Particle &particle, vector<VectorXf> z, MatrixXf R);
 
#endif //ADD_FEATURE_H

