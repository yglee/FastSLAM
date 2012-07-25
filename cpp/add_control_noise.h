#ifndef ADD_CONTROL_NOISE
#define ADD_CONTROL_NOISE

#include <Eigen/Dense>
#include "multivariate_gauss.h"

using namespace Eigen;

void add_control_noise(float &V, float &G, Matrix2f Q, int addnoise);

#endif //ADD_CONTROL_NOISE
