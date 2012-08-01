#ifndef COMPUTE_JACOBIANS_H
#define COMPUTE_JACOBIANS_H

#include <Eigen/Dense>
#include "std"

using namespace std;
using namespace Eigen;

void compute_jacobians(Particle particle, vector<int> idf, MatrixXf R);

#endif //COMPUTE_JACOBIANS_H
