#ifndef ADD_OBSERVATION_NOISE_H
#define ADD_OBSERVATION_NOISE_H

#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

void add_observation_noise(MatrixXf &z, MatrixXf R, int addnoise);

#endif //ADD_OBSERVATION_NOISE_H
