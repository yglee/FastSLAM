#include "fastslam2_sim.h"
#include "particles.h"

using namespace config;

void fastslam2_sim(MatrixXd lm, MatrixXd wp) 
{
	if (SWITCH_PREDICT_NOISE) {
		printf("Sampling from predict noise usually OFF for FastSLAM 2.0\n");	
	}
	if (SWITCH_SAMPLE_PROPOSAL == 0) {
		printf("Sampling from optimal proposal is usually ON for FastSLAM 2.0\n");
	}

	Q << sigmaV*sigmaV, 0,
		 0 , sigmaG*sigmaG;

	R << sigmaR*sigmaR, 0, 
		 0, sigmaB*sigmaB;

	float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};
	
	Particles* particles = new Particles(NPARTICLES);
	VectorXf xtrue(3);
	xtrue.setZero();
	
	float dt = DT_CONTROLS; //change in time btw predicts
	float dtsum = 0; //change in time since last observation
	VectorXf ftag(lm.cols());

	for (unsigned i=0; i< lm.cols(); i++) {
		ftag[i] = i+1; //ftag items are indexed from 1
	}
 
	VectorXf da_table(lm.cols());
	da_table.setZero();	
	
	int iwp = 1; //index to first waypoint
	int G = 0; //initial steer angle
	float* plines = NULL; //will later change to list of points

	//TODO: double check this line
	if (SWITCH_SEED_RANDOM !=0) {
		srand(SWITCH_SEED_RANDOM);
	} 		

	MatrixXd Qe = MatrixXd(Q);
	MatrixXd Re = MatrixXd(R);

	if (SWITCH_INFLATE_NOISE ==1) {
		Qe = 2*Q;
		Re = 2*R;
	}

	if (SWITCH_PROFILE) {
		//TODO: 
	}	
	
	while (iwp !=0) {
		//compute_steering();
	}
}
