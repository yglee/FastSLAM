#ifndef GET_OBSERVATIONS_H
#define GET_OBSERVATIONS_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

vector<VectorXf> get_observations(VectorXf x,MatrixXf lm,vector<int> &idf,float rmax);
void get_visible_landmarks(VectorXf x, MatrixXf &lm,vector<int> &idf, float rmax);
vector<VectorXf> compute_range_bearing(VectorXf x, MatrixXf lm);
vector<int> find2(vector<float> dx, vector<float> dy, float phi, float rmax);

#endif
