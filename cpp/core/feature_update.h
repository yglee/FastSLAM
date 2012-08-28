#ifndef FEATURE_UPDATE_H
#define FEATURE_UPDATE_H

#include <Eigen/Dense>
#include <vector>

#include "particle.h"
#include "compute_jacobians.h"
#include "pi_to_pi.h"
#include "KF_cholesky_update.h"

using namespace Eigen;
using namespace std;


void feature_update(Particle &particle, vector<VectorXf> z, vector<int>idf, MatrixXf R);

#endif
