#ifndef PREDICT_H
#define PREDICT_H

void predict(Particle &particle,float V,float G,MatrixXd Q, \
			float WB,float dt, int addrandom);

#endif //PREDICT_H
