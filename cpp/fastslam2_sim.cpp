#include "fastslam2_sim.h"
#include "particles.h"
#include "configfile.h"
#include <stdlib.h>
#include "printMat.h"

#include <boost/numeric/ublas/vector.hpp>  
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas = boost::numeric::ublas;

void fastslam2_sim(ublas::matrix<float> lm, ublas::matrix<float> wp) 
{
	if (SWITCH_PREDICT_NOISE) {
		printf("Sampling from predict noise usually OFF for FastSLAM 2.0\n");	
	}
	if (SWITCH_SAMPLE_PROPOSAL == 0) {
		printf("Sampling from optimal proposal is usually ON for FastSLAM 2.0\n");
	}
	
	float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};
	
	Particles* particles = new Particles(NPARTICLES);
	ublas::zero_vector<float> xtrue (3);
	printf("[%f %f %f]\n",xtrue(0),xtrue(1),xtrue(2));
	
	float dt = DT_CONTROLS; //change in time btw predicts
	float dtsum = 0; //change in time since last observation
	std::vector<int> ftag(lm.size2()); //identifier for each landmark
	
	for (unsigned i=0; i< lm.size2(); i++) {
		ftag.insert_element(i,i+1); //ftag is indexed from 1
	} 
	ublas::zero_vector<int> da_table(lm.size2()); //data association table
	int iwp = 1; //index to first waypoint
	int G = 0; //initial steer angle
	float* plines = NULL; //will later change to list of points

	//TODO: double check this line
	if (SWITCH_SEED_RANDOM !=0) {
		srand(SWITCH_SEED_RANDOM);
	} 		

	//ublas::matrix<float> Qe = matrix(Q);
	//printMat(Qe);
}
