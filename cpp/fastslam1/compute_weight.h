#ifndef COMPUTE_WEIGHT_H
#define COMPTUE_WEIGHT_H

#include <Eigen/Dense>
#include <vector>
#include "core/particle.h"

using namespace Eigen;
using namespace std;

float compute_weight(Particle &particle, vector<VectorXf> z, vector<int> idf, MatrixXf R); 

#endif
