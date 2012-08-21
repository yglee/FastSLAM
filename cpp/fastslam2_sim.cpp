#include <iostream>
#include <math.h>
#include <vector>

#include "fastslam2_sim.h"
#include "add_control_noise.h"
#include "predict.h"
#include "observe_heading.h"
#include "get_observations.h"
#include "add_observation_noise.h"
#include "TransformToGlobal.h"
#include "line_plot_conversion.h"
#include "data_associate_known.h"
#include "sample_proposal.h"
#include "feature_update.h"
#include "resample_particles.h"
#include "add_feature.h"

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
    MatrixXf plines; //will later change to list of points

    if (SWITCH_SEED_RANDOM !=0) {
        srand(SWITCH_SEED_RANDOM);
    } 		

    MatrixXf Qe = MatrixXf(Q);
    MatrixXf Re = MatrixXf(R);

    if (SWITCH_INFLATE_NOISE ==1) {
        Qe = 2*Q;
        Re = 2*R;
    }

    if (SWITCH_PROFILE) {
        //TODO: 
    }	

    vector<int> ftag_visible;
    MatrixXf z;
    int count =0;
    
    //Main loop
    while (iwp !=-1) {
        compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
        if (iwp ==-1 && NUMBER_LOOPS > 1) {
            iwp = 0;
            NUMBER_LOOPS = NUMBER_LOOPS-1;
        }
        predict_true(xtrue,V,G,WHEELBASE,dt);

        //add process noise
        //TODO: need to truly randomize function in multivariate_gauss
        float* VnGn = new float[2];
        //add_control_noise(V,G,Q,SWITCH_CONTROL_NOISE,VnGn);
        float Vn = V;//VnGn[0];
        float Gn = G;//VnGn[1];

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
            z = get_observations(xtrue,lm,ftag_visible,MAX_RANGE);
            //TODO
            //add_observation_noise(z,R,SWITCH_SENSOR_NOISE);
            if(!z.isZero()){
                plines = make_laser_lines(z,xtrue);
            }

            //Compute (known) data associations
            int Nf = (particles[0].xf()).cols();
            vector<int> idf;
            MatrixXf zf(z.rows(),ftag_visible.size());
            zf.setZero();
            MatrixXf zn(z.rows(),ftag_visible.size());
            zn.setZero();
            data_associate_known(z,ftag_visible,da_table,Nf,zf,idf,zn);	

            //observe map features
            if (!zf.isZero()) {
                //isample from 'optimal' proposal distribution, then update map
                for (unsigned i=0; i<NPARTICLES; i++) {
                    sample_proposal(particles[i], zf, idf, Re);
                    feature_update(particles[i],zf,idf,Re);
                }

                //resample
                resample_particles(particles,NEFFECTIVE,SWITCH_RESAMPLE);
            }

            //Observe new features, augment map
            //TODO: xv gets depreicated here
            if (!zn.isZero()) {
                for (unsigned i=0; i<NPARTICLES; i++) {
                    if (zf.isZero()) {//sample from proposal distribution if we have not already done so above
                        VectorXf xv = multivariate_gauss(particles[i].xv(),
                                                        particles[i].Pv(),1);
                        //TODO: xv[0] seems to have an approximation error from chol.
                        particles[i].setXv(xv);
                        MatrixXf pv(3,3); 
                        pv.setZero();
                        particles[i].setPv(pv); 
                    }

                    add_feature(particles[i], zn, Re);	
                }
            }	
            count++;
            if (VnGn) { 
                delete[] VnGn;
            }
        }
    }
    cout<<"done with all functions and will return particles"<<endl;
    return particles;
}

MatrixXf make_laser_lines(MatrixXf rb, VectorXf xv) 
{
    if (rb.isZero()) {
        return MatrixXf(0,0);
    }

    int len = rb.cols();
    MatrixXf lnes(4,len);

    MatrixXf globalMat(2,rb.cols());
    int j;
    for (j=0; j<globalMat.cols();j++) {
        globalMat(0,j) = rb(0,j)*cos(rb(1,j));
        globalMat(1,j) = rb(0,j)*sin(rb(1,j));   	
    }

    TransformToGlobal(globalMat,xv);

    for (int c=0; c<lnes.cols();c++) {
        lnes(0,c) = xv(0);
        lnes(1,c) = xv(1);
        lnes(2,c) = globalMat(0,c);
        lnes(3,c) = globalMat(1,c);
    }

    return line_plot_conversion(lnes);
}
