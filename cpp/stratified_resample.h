#ifndef STRATIFIED_RESAMPLE_H
#define STRATIFIED_RESAMPLE_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

void stratified_resample(VectorXf w, vector<int> &keep, float &Neff);
void cumsum(VectorXf &w);

#endif
