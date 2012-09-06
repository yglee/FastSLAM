#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <vector>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

void read_input_file(const string s, MatrixXf *lm, MatrixXf *wp); 