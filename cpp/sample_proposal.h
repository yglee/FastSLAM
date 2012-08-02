#ifndef SAMPLE_PROPOSAL_H
#define SAMPLE_PROPOSAL_H

#include <vector>
#include <Eigen/Dense>
#include "particle.h"

using namespace Eigen;
using namespace std;

void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R);
VectorXf likelihood_given_xv(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R);
VectorXf delta_xv(VectorXf xv1, VectorXf xv2)
#endif
