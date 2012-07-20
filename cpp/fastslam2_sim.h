#ifndef FASTSLAM2_SIM_H
#define FASTSLAM2_SIM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <eigen3/Eigen/Dense>

#include "configfile.h"

using namespace Eigen;

void fastslam2_sim(MatrixXd lm, MatrixXd wp);

#endif //FASTSLAM2_SIM_H
