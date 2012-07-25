#include "configfile.h"

#define pi 3.14159265

// Configuration file
//Permits various adjustments to parameters of the FastSLAM algorithm.
// See fastslam_sim.h for more information

// control parameters
int config::V= 3; // m/s
float config::MAXG= 30*pi/180; // radians, maximum steering angle (-MAXG < g < MAXG)
float config::RATEG= 20*pi/180; // rad/s, maximum rate of change in steer angle
int config::WHEELBASE= 4; // metres, vehicle wheel-base
float config::DT_CONTROLS= 0.025; // seconds, time interval between control signals

// control noises
float config::sigmaV= 0.3; // m/s
float config::sigmaG= (3.0*pi/180); // radians

Eigen::Matrix2f config::Q;

// observation parameters
float config::MAX_RANGE= 30.0; // metres
float config::DT_OBSERVE= 8* config::DT_CONTROLS; // seconds, time interval between observations

// observation noises
float config::sigmaR= 0.1; // metres
float config::sigmaB= (1.0*pi/180); // radians

Eigen::Matrix2f config::R;

// waypoint proximity
float config::AT_WAYPOINT= 1.0; // metres, distance from current waypoint at which to switch to next waypoint
int config::NUMBER_LOOPS= 2; // number of loops through the waypoint list

// resampling
unsigned int config::NPARTICLES= 100; 
float config::NEFFECTIVE= 0.75* config::NPARTICLES; // minimum number of effective particles before resampling

// switches
int config::SWITCH_CONTROL_NOISE= 1;
int config::SWITCH_SENSOR_NOISE = 1;
int config::SWITCH_INFLATE_NOISE= 0;
int config::SWITCH_PREDICT_NOISE = 0;   // sample noise from predict (usually 1 for fastslam1.0 and 0 for fastslam2.0)
int config::SWITCH_SAMPLE_PROPOSAL = 1; // sample from proposal (no effect on fastslam1.0 and usually 1 for fastslam2.0)
int config::SWITCH_HEADING_KNOWN= 0;
int config::SWITCH_RESAMPLE= 1; 
int config::SWITCH_PROFILE= 1;
int config::SWITCH_SEED_RANDOM= 0; // if not 0, seed the randn() with its value at beginning of simulation (for repeatability)

