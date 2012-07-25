#ifndef COMPUTE_STEERING_H
#define COMPUTE_STEERING_H

#include <Eigen/Dense>

using namespace Eigen;

void compute_steering(VectorXf x, MatrixXf wp, int& iwp, float minD, 
						float& G, float rateG, float maxG, float dt);

#endif //COMPUTE_STEERING_H
