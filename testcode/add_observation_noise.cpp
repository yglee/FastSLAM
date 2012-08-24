#include "add_observation_noise.h"

//add random measurement noise. We assume R is diagnoal matrix
void add_observation_noise(MatrixXf &z, MatrixXf R, int addnoise)
{
	if (addnoise == 1){
		int len = z.cols();	
		if (len > 0) {
			MatrixXf randM1(1,len);
			MatrixXf randM2(1,len);
			randM1.setRandom();
			randM2.setRandom();
				
			for(int c=0; c<len; c++) {
				z(0,c) = z(0,c) + randM1(0,c)*sqrt(R(0,0));
				z(1,c) = z(1,c) + randM2(0,c)*sqrt(R(1,1));
			}
		}
	}	
}
