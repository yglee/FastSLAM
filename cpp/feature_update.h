#ifndef FEATURE_UPDATE_H
#define FEATURE_UPDATE_H

#include <Eigen/Dense>

using namespace Eigen;
void feature_update(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R);

#endif
