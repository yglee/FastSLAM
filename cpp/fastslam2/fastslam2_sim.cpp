#include <iostream>
#include <math.h>
#include <vector>

#include "fastslam2_sim.h"
#include "core/add_control_noise.h"
#include "predict.h"
#include "observe_heading.h"
#include "core/get_observations.h"
#include "core/add_observation_noise.h"
#include "core/TransformToGlobal.h"
#include "core/line_plot_conversion.h"
#include "core/data_associate_known.h"
#include "sample_proposal.h"
#include "core/feature_update.h"
#include "core/resample_particles.h"
#include "core/add_feature.h"

using namespace config;
using namespace std;

vector<Particle> fastslam2_sim(MatrixXf lm, MatrixXf wp) 
{
    if (SWITCH_PREDICT_NOISE) {
        printf("Sampling from predict noise usually OFF for FastSLAM 2.0\n");	
    }
    if (SWITCH_SAMPLE_PROPOSAL == 0) {
        printf("Sampling from optimal proposal is usually ON for FastSLAM 2.0\n");
    }
    //normally initialized configfile.h
    Q << pow(sigmaV,2), 0,
      0 , pow(sigmaG,2);

    R << sigmaR*sigmaR, 0, 
      0, sigmaB*sigmaB;

    float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};

    //vector of particles (their count will change)
    vector<Particle> particles(NPARTICLES);
    for (int i=0; i<particles.size(); i++){
        particles[i] = Particle();
    }
   
    //initialize particle weights as uniform
    float uniformw = 1.0/(float)NPARTICLES;    
    for (unsigned int p = 0; p < NPARTICLES; p++) {
	    particles[p].setW(uniformw);
    }

    VectorXf xtrue(3);
    xtrue.setZero();

    float dt = DT_CONTROLS; //change in time btw predicts
    float dtsum = 0; //change in time since last observation

    vector<int> ftag; //identifier for each landmark
    for (int i=0; i< lm.cols(); i++) {
        ftag.push_back(i); 
    }

    //data ssociation table
    VectorXf da_table(lm.cols());
    for (int s=0; s<da_table.size(); s++) {
        da_table[s] = -1;
    }

    int iwp = 0; //index to first waypoint
    float G = 0; //initial steer angle
    //MatrixXf plines; //will later change to list of points

    if (SWITCH_SEED_RANDOM !=0) {
        srand(SWITCH_SEED_RANDOM);
    } 		

    MatrixXf Qe = MatrixXf(Q);
    MatrixXf Re = MatrixXf(R);

    if (SWITCH_INFLATE_NOISE ==1) {
        Qe = 2*Q;
        Re = 2*R;
    }

    vector<int> ftag_visible;
   	vector<VectorXf> z;
 
    //Main loop
    while (iwp !=-1) {
        compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
        if (iwp ==-1 && NUMBER_LOOPS > 1) {
            iwp = 0;
            NUMBER_LOOPS = NUMBER_LOOPS-1;
        }
        predict_true(xtrue,V,G,WHEELBASE,dt);

        //add process noise
        float* VnGn = new float[2];
        add_control_noise(V,G,Q,SWITCH_CONTROL_NOISE,VnGn);
        float Vn = VnGn[0];
        float Gn = VnGn[1];

        //Predict step	
        for (unsigned int i=0; i< NPARTICLES; i++) {
            predict(particles[i],Vn,Gn,Qe,WHEELBASE,dt,SWITCH_PREDICT_NOISE);
            observe_heading(particles[i], xtrue(2), SWITCH_HEADING_KNOWN); //if heading known, observe heading
        }

        //Observe step
        dtsum = dtsum+dt;
        if (dtsum >= DT_OBSERVE) {
            dtsum=0;

            //Compute true data, then add noise
            ftag_visible = vector<int>(ftag); //modify the copy, not the ftag	
            
            //z is the range and bearing of the observed landmark
            z = get_observations(xtrue,lm,ftag_visible,MAX_RANGE);
            add_observation_noise(z,R,SWITCH_SENSOR_NOISE);
            
	    if(!z.empty()){
                //plines = make_laser_lines(z,xtrue);
            }

            //Compute (known) data associations
            int Nf = particles[0].xf().size(); //(particles[0].xf()).cols();
            vector<int> idf;
            vector<VectorXf> zf;
	    vector<VectorXf> zn;

            bool testflag= false;
            data_associate_known(z,ftag_visible,da_table,Nf,zf,idf,zn);
            
			//observe map features
            if (!zf.empty()) {
                //isample from 'optimal' proposal distribution, then update map
                for (unsigned i=0; i<NPARTICLES; i++) {
                    sample_proposal(particles[i], zf, idf, Re);
                    feature_update(particles[i], zf, idf, Re);
                }
                //resample
                resample_particles(particles,NEFFECTIVE,SWITCH_RESAMPLE);
            }

            //Observe new features, augment map
            if (!zn.empty()) {//!zn.isZero()) {
                for (unsigned i=0; i<NPARTICLES; i++) {
                    if (zf.empty()) {//zf.isZero()) {//sample from proposal distribution if we have not already done so above
                        VectorXf xv = multivariate_gauss(particles[i].xv(),
                                                        particles[i].Pv(),1);
                        particles[i].setXv(xv);
                        MatrixXf pv(3,3); 
                        pv.setZero();
                        particles[i].setPv(pv); 
                    }
                    add_feature(particles[i], zn, Re);
                }
            }	
            
			if (VnGn) { 
                delete[] VnGn;
            }
        }
    }
    cout<<"done with all functions and will return particles"<<endl<<flush;
    return particles;
}

