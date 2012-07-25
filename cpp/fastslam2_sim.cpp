#include "fastslam2_sim.h"
#include "particles.h"
#include "add_control_noise.h"
#include <iostream>
#include <math.h>

using namespace config;

void fastslam2_sim(MatrixXf lm, MatrixXf wp) 
{
	if (SWITCH_PREDICT_NOISE) {
		printf("Sampling from predict noise usually OFF for FastSLAM 2.0\n");	
	}
	if (SWITCH_SAMPLE_PROPOSAL == 0) {
		printf("Sampling from optimal proposal is usually ON for FastSLAM 2.0\n");
	}

	Q << pow(sigmaV,2), 0,
		 0 , pow(sigmaG,2);

	R << sigmaR*sigmaR, 0, 
		 0, sigmaB*sigmaB;

	float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};
	
	Particles* particles = new Particles(NPARTICLES);
	VectorXf xtrue(3);
	xtrue.setZero();
	
	float dt = DT_CONTROLS; //change in time btw predicts
	float dtsum = 0; //change in time since last observation
	VectorXf ftag(lm.cols());

	for (int i=0; i< lm.cols(); i++) {
		ftag[i] = i+1; //ftag items are indexed from 1
	}
 
	VectorXf da_table(lm.cols());
	da_table.setZero();	
	
	int iwp = 1; //index to first waypoint
	float G = 0; //initial steer angle
	float* plines = NULL; //will later change to list of points

	if (SWITCH_SEED_RANDOM !=0) {
		srand(SWITCH_SEED_RANDOM);
	} 		

	Matrix2f Qe = Matrix2f(Q);
	Matrix2f Re = Matrix2f(R);

	if (SWITCH_INFLATE_NOISE ==1) {
		Qe = 2*Q;
		Re = 2*R;
	}

	if (SWITCH_PROFILE) {
		//TODO: 
	}	
	
	while (iwp !=0) {
		//compute true data
		compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
		if (iwp ==0 && NUMBER_LOOPS > 1) {
			iwp = 1;
			NUMBER_LOOPS = NUMBER_LOOPS-1;
		}
		predict_true(xtrue,V,G,WHEELBASE,dt);
		
		//add process noise noise
		//TODO: need to truly randomize function in multivariate_gauss
		add_control_noise(V,G,Q,SWITCH_CONTROL_NOISE);
	
		//predict step	
		for (int i=0; i< NPARTICLES; i++) {
			particles[i] = predict(particles[i],Vn,Gn,Qe,WHEELBASE,dt,SWITCH_PREDICT_NOISE);
			if (SWITCH_HEADING_KNOWN ==1) {
				particles[i].xf[2] = xtrue[2];
			}
		}	
	}		

}
