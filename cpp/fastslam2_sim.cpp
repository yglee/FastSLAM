#include "fastslam2_sim.h"
#include "configfile.h"

#define SWITCH_PREDICT_NOISE 0
#define SWITCH_SAMPLE_PROPOSAL 0

void fastslam2_sim(float* lm, float* wp) 
{
	if (SWITCH_PREDICT_NOISE) {
		printf("Sampling from predict noise usually OFF for FastSLAM 2.0\n");	
	}
	if (SWITCH_SAMPLE_PROPOSAL == 0) {
		printf("Sampling from optimal proposal is usually ON for FastSLAM 2.0\n");
	}
	
	float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};
	
	//particles = initialize_particles(NPARTICLES);	
}
