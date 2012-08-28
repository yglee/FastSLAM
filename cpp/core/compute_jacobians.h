#ifndef COMPUTE_JACOBIANS_H
#define COMPUTE_JACOBIANS_H

#include <Eigen/Dense>
#include "particle.h"
#include <vector>

using namespace std;
using namespace Eigen;

void compute_jacobians(Particle particle, 
                        vector<int> idf, 
                        MatrixXf R,
                        vector<VectorXf> &zp,
						vector<MatrixXf> *Hv, 
                        vector<MatrixXf> *Hf, 
                        vector<MatrixXf> *Sf);

#endif //COMPUTE_JACOBIANS_H
