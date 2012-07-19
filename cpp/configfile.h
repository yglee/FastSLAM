#define pi 3.14159265

// Configuration file
//Permits various adjustments to parameters of the FastSLAM algorithm.
// See fastslam_sim.h for more information

// control parameters
const int V= 3; // m/s
const float MAXG= 30*pi/180; // radians, maximum steering angle (-MAXG < g < MAXG)
const float RATEG= 20*pi/180; // rad/s, maximum rate of change in steer angle
const int WHEELBASE= 4; // metres, vehicle wheel-base
const float DT_CONTROLS= 0.025; // seconds, time interval between control signals

// control noises
const float sigmaV= 0.3; // m/s
const float sigmaG= (3.0*pi/180); // radians
//Q= [sigmaV^2 0; 0 sigmaG^2];

// observation parameters
const float MAX_RANGE= 30.0; // metres
const float DT_OBSERVE= 8*DT_CONTROLS; // seconds, time interval between observations

// observation noises
const float sigmaR= 0.1; // metres
const float sigmaB= (1.0*pi/180); // radians
//R= [sigmaR^2 0; 0 sigmaB^2];

// waypoint proximity
const float AT_WAYPOINT= 1.0; // metres, distance from current waypoint at which to switch to next waypoint
const int NUMBER_LOOPS= 2; // number of loops through the waypoint list

// resampling
const int NPARTICLES= 100; 
const float NEFFECTIVE= 0.75*NPARTICLES; // minimum number of effective particles before resampling

// switches
const int SWITCH_CONTROL_NOISE= 1;
const int SWITCH_SENSOR_NOISE = 1;
const int SWITCH_INFLATE_NOISE= 0;
const int SWITCH_PREDICT_NOISE = 0;   // sample noise from predict (usually 1 for fastslam1.0 and 0 for fastslam2.0)
const int SWITCH_SAMPLE_PROPOSAL = 1; // sample from proposal (no effect on fastslam1.0 and usually 1 for fastslam2.0)
const int SWITCH_HEADING_KNOWN= 0;
const int SWITCH_RESAMPLE= 1; 
const int SWITCH_PROFILE= 1;
const int SWITCH_SEED_RANDOM= 0; // if not 0, seed the randn() with its value at beginning of simulation (for repeatability)

