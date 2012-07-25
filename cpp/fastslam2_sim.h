#ifndef FASTSLAM2_SIM_H
#define FASTSLAM2_SIM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <Eigen/Dense>

#include "configfile.h"
#include "compute_steering.h"
#include "predict_true.h"

using namespace Eigen;

void fastslam2_sim(MatrixXf lm, MatrixXf wp);

#endif //FASTSLAM2_SIM_H
