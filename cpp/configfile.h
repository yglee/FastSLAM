#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <eigen3/Eigen/Dense>

//******************
// Global Variables
//******************

namespace config {
		extern int V;
		extern float MAXG;
		extern float RATEG;
		extern int WHEELBASE;
		extern float DT_CONTROLS;

		extern float sigmaV;
		extern float sigmaG;

		extern Eigen::Matrix2d Q;

		extern float MAX_RANGE;
		extern float DT_OBSERVE;

		extern float sigmaR;
		extern float sigmaB;
		
		extern Eigen::Matrix2d R;

		extern float AT_WAYPOINT;
		extern int NUMBER_LOOPS;

		extern unsigned int NPARTICLES;
		extern float NEFFECTIVE;

		extern int SWITCH_CONTROL_NOISE;
		extern int SWITCH_SENSOR_NOISE;
		extern int SWITCH_INFLATE_NOISE;
		extern int SWITCH_PREDICT_NOISE;

		extern int SWITCH_SAMPLE_PROPOSAL;
		extern int SWITCH_HEADING_KNOWN;
		extern int SWITCH_RESAMPLE;
		extern int SWITCH_PROFILE;
		extern int SWITCH_SEED_RANDOM;
}
#endif //CONFIGFILE_H
