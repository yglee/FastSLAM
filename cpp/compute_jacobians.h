#ifndef COMPUTE_JACOBIANS_H
#define COMPUTE_JACOBIANS_H

#include <Eigen/Dense>
#include "particle.h"
#include <vector>

using namespace std;
using namespace Eigen;

void compute_jacobians(Particle particle, vector<int> idf, MatrixXf R, \
                       MatrixXf &zp, MatrixXf *Hv, MatrixXf *Hf, MatrixXf *Sf);

#endif //COMPUTE_JACOBIANS_H
