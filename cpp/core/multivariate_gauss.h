#ifndef MULTIVARIATE_GAUSS_H
#define MULTIVARIATE_GAUSS_H

#include <Eigen/Dense>

using namespace Eigen;

VectorXf multivariate_gauss(VectorXf x, MatrixXf P, int n);

#endif //MULTIVARIATE_GAUSS_H

