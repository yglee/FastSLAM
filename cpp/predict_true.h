#ifndef PREDICT_TRUE_H
#define PREDICT_TRUE_H

#include <Eigen/Dense>
#include "pi_to_pi.h"

using namespace Eigen;

void predict_true(VectorXf &xv,float V,float G,int WB,float dt); 
//V is m/s
// G is steering angle
//WB = WHEELBASE	 
	
#endif //PREDICT_TRUE_H
