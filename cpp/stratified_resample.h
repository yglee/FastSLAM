#ifndef STRATIFIED_RESAMPLE_H
#define STRATIFIED_RESAMPLE_H

#include <Eigen/Dense>

using namespace Eigen;

void stratified_resample(VectorXf w, VectorXf &keep, float &Neff);

#endif
