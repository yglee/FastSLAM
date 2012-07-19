#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#define pi 3.14159265

using namespace boost::numeric::ublas;

// Configuration file
//Permits various adjustments to parameters of the FastSLAM algorithm.
// See fastslam_sim.h for more information

// control parameters
int V= 3; // m/s
float MAXG= 30*pi/180; // radians, maximum steering angle (-MAXG < g < MAXG)
float RATEG= 20*pi/180; // rad/s, maximum rate of change in steer angle
int WHEELBASE= 4; // metres, vehicle wheel-base
float DT_CONTROLS= 0.025; // seconds, time interval between control signals

// control noises
float sigmaV= 0.3; // m/s
float sigmaG= (3.0*pi/180); // radians

matrix<float> Q;
Q.insert_element(0,0,sigmaV*sigmaV);
Q.insert_element(0,1,0); 
Q.insert_element(1,0,0); 
Q.insert_element(1,1,sigmaG*sigmaG);
 
// observation parameters
float MAX_RANGE= 30.0; // metres
float DT_OBSERVE= 8*DT_CONTROLS; // seconds, time interval between observations

// observation noises
float sigmaR= 0.1; // metres
float sigmaB= (1.0*pi/180); // radians

matrix<float> R; 
R.insert_element(0,0,sigmaR*sigmaR); 
R.insert_element(0,1,0); 
R.insert_element(1,0,0);
R.insert_element(1,1,sigmaB*sigmaB);
 
// waypoint proximity
float AT_WAYPOINT= 1.0; // metres, distance from current waypoint at which to switch to next waypoint
int NUMBER_LOOPS= 2; // number of loops through the waypoint list

// resampling
unsigned int NPARTICLES= 100; 
float NEFFECTIVE= 0.75*NPARTICLES; // minimum number of effective particles before resampling

// switches
int SWITCH_CONTROL_NOISE= 1;
int SWITCH_SENSOR_NOISE = 1;
int SWITCH_INFLATE_NOISE= 0;
int SWITCH_PREDICT_NOISE = 0;   // sample noise from predict (usually 1 for fastslam1.0 and 0 for fastslam2.0)
int SWITCH_SAMPLE_PROPOSAL = 1; // sample from proposal (no effect on fastslam1.0 and usually 1 for fastslam2.0)
int SWITCH_HEADING_KNOWN= 0;
int SWITCH_RESAMPLE= 1; 
int SWITCH_PROFILE= 1;
int SWITCH_SEED_RANDOM= 0; // if not 0, seed the randn() with its value at beginning of simulation (for repeatability)

