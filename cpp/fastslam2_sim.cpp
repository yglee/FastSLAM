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

    Q << pow(sigmaV,2), 0,
      0 , pow(sigmaG,2);

    R << sigmaR*sigmaR, 0, 
      0, sigmaB*sigmaB;

    float veh[2][3] = {{0,-WHEELBASE,-WHEELBASE},{0,-1,1}};

    //vector of particles (their count will change)
    vector<Particle> particles(NPARTICLES);

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
    da_table.setZero();	

    int iwp = 1; //index to first waypoint
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
    while (iwp !=0) {
        //compute true data
        compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
        if (iwp ==0 && NUMBER_LOOPS > 1) {
            iwp = 1;
            NUMBER_LOOPS = NUMBER_LOOPS-1;
        }
        predict_true(xtrue,V,G,WHEELBASE,dt);

        //add process noise
        //TODO: need to truly randomize function in multivariate_gauss
        float* VnGn = new float[2];
        add_control_noise(V,G,Q,SWITCH_CONTROL_NOISE,VnGn);
        float Vn = VnGn[0];
        float Gn = VnGn[1];

        //predict step	
        for (unsigned int i=0; i< NPARTICLES; i++) {
            predict(particles[i],Vn,Gn,Qe,WHEELBASE,dt,SWITCH_PREDICT_NOISE);
            observe_heading(particles[i], xtrue(2), SWITCH_HEADING_KNOWN); //if heading known, observe heading
        }

#if 0
        cout<<"inside fastslam2"<<endl;
        cout<<"particle count: "<<particles.size()<<endl;
        cout<<"w: "<<particles[0].w()<<" " <<"should be 0.0100"<<endl;
        cout<<"xv: "<<particles[0].xv()<<" "<<"should be 0.0824, 0.0036, 0.0009"<<endl;
        cout<<"Pv: "<<particles[0].Pv()<<" "<<"should be 1.0e-0.4 *[0.5618, 0.0164, 0.0041; 0.0164, 0.1873, 0.0468; 0.0041, 0.0468, 0.0117]" <<endl;
        cout<<"xf: "<<particles[0].xf()<<" "<<"should be empty"<<endl;
        //cout<<"Pf: "<<particles[0].Pf()[0]<<" "<<"should be empty"<<endl;
        //cout<<"da: "<<particles[0].da()[0]<<" "<<"should be empty"<<endl;
#endif

        //observe step
        dtsum = dtsum+dt;	
        if (dtsum >= DT_OBSERVE) {
            dtsum=0;

            //compute true data, then add noise
            ftag_visible = vector<int>(ftag); //modify the copy, not the ftag	
            z = get_observations(xtrue,lm,ftag_visible,MAX_RANGE);
#if 0
            cout<<"ftag"<<endl;
            vector<int>::iterator iter;
            for (iter =ftag.begin(); iter!=ftag.end(); iter++) {
                cout<<(*iter)<<" ";
            }
            cout<<endl;	

            //z = get_observations(xtrue,lm,ftag_visible,MAX_RANGE);
            cout<<"ftag_visible"<<endl;
            vector<int>::iterator iter2;
            for (iter2 =ftag_visible.begin(); iter2!=ftag_visible.end(); iter2++) {
                cout<<(*iter2)<<" ";
            }
            cout<<endl;
            cout<<"ftag_visible should be 0,21"<<endl;	

            //TODO:commented out for testing
            //add_observation_noise(z,R,SWITCH_SENSOR_NOISE);
            cout<<"get_observations returns z: "<<endl;
            cout<<z<<endl;
            cout<<"z should be [25.7745 25.2762; -1.4734,0.1384]"<<endl;
#endif

            if(!z.isZero()){
                plines = make_laser_lines(z,xtrue);
            }

#if 0
            cout<<"plines "<<endl;
            cout<<plines<<endl;
            cout<<"plines should be [0.6741, 2.9922, Nan, 0.6741, 25.7446; -0.0309,-25.7009,Nan, -0.0309, 3.2710]"<<endl;
#endif

            //compute (known) data associations
            int Nf = (particles[0].xf()).cols();

            vector<int> idf;
            MatrixXf zf(z.rows(),ftag_visible.size());
            zf.setZero();
            MatrixXf zn(z.rows(),ftag_visible.size());
            zn.setZero();

            data_associate_known(z,ftag_visible,da_table,Nf,zf,idf,zn);	

            //cout<<"in Fastslam2: idf size() "<<idf.size()<<endl;
            
#if 0
            cout<<"zf "<<zf<<endl;
            cout<<"zf should be empty"<<endl;

            cout<<"idf";	
            vector<int>::iterator iter;
            for (iter=idf.begin(); iter!=idf.end(); iter++) {
                cout<<(*iter)<<" ";
            }
            cout<<endl;
            cout<<"idf should be empty"<<endl;

            cout<<"zn"<<endl;
            cout<<zn<<endl;
            cout<<"zn should be [25.7745, 25.2762; -1.4734, 0.1384]"<<endl;

            cout<<"da_table"<<endl;
            cout<<da_table<<endl;
            //cout<<"da_table[0] = "<<da_table[0]<<" da_table[21]="<<da_table[21]<<endl;
            cout<<"da_table should be all zero except 0 at index 0, 1 at index 21"<<endl;
#endif

            //observe map features
            if (!zf.isZero()) {
#if 0
                cout<<"zf is "<<endl;
                cout<<zf<<endl;
                cout<<"zf should be 25.6305 24.6263"<<endl;
                cout<<"             -1.4785 0.1662"<<endl;
#endif
                //isample from 'optimal' proposal distribution, then update map

                for (unsigned i=0; i<NPARTICLES; i++) {
                    sample_proposal(particles[i], zf, idf, Re);
                    feature_update(particles[i],zf,idf,Re);
                }	

#if 0	
                cout<<"After sample_proposal and feature_udpate"<<endl;	
                cout<<"particle count: "<<particles.size()<<endl;
                cout<<"w: "<<particles[0].w()<<" " <<"should be 14.7297"<<endl;
                cout<<"xv: "<<particles[0].xv()<<endl;
                cout<<"should be 1.5256"<<endl;
                cout<<"          -0.0182"<<endl;
                cout<<"          -0.0052"<<endl;
                cout<<endl;
                cout<<"Pv: "<<particles[0].Pv()<<" "<<"should be all zeros" <<endl;
                cout<<"xf: "<<particles[0].xf()<<endl;
                cout<<"should be 3.5838, 26.8077; -25.5826,3.7884"<<endl;	
                cout<<endl;				
                cout<<"Pf[0]: "<<particles[0].Pf()[0]<<endl;
                cout<<"should be [0.0996, 0.0083; 0.0083 0.0057]"<<endl;
                cout<<endl;
                cout<<"Pf[1]: "<<particles[0].Pf()[1]<<endl;
                cout<<"should be [0.0069, -0.0129; -0.0129, 0.0925"<<endl;
                cout<<endl;
                //cout<<"Pf: "<<particles[0].Pf()[0]<<" "<<"should be empty"<<endl;
                //cout<<"da: "<<particles[0].da()[0]<<" "<<"should be empty"<<endl;
#endif

                //resample
                resample_particles(particles,NEFFECTIVE,SWITCH_RESAMPLE);
            }

            //Observe new features, augment map
            //TODO: xv gets depreicated here
            if (!zn.isZero()) {
                for (unsigned i=0; i<NPARTICLES; i++) {
                    if (zf.isZero()) {//sample from proposal distribution if we have not already done so above
                        #if 0
                        cout<<"in zf IS ZERO"<<endl;
                        cout<<"particles["<<i<<"].xv()"<<particles[i].xv()<<endl;
                        cout<<"particles["<<i<<"].Pv()"<<endl;
                        cout<<particles[i].Pv()<<endl;
                        #endif
                        VectorXf xv = multivariate_gauss(particles[i].xv(),
                                particles[i].Pv(),1);

                        //TODO: xv[0] seems to have an approximation error from chol.
                        particles[i].setXv(xv);
                        #if 0
                        cout<<"xv in fastslam2"<<endl;
                        cout<<xv<<endl;
                        cout<<"should be"<<endl;
                        cout<<"0.7648"<<endl;
                        cout<<"0.0221"<<endl;
                        cout<<"0.0050"<<endl;
                        cout<<endl;
                        #endif
                        MatrixXf pv(3,3); 
                        pv.setZero();
                        particles[i].setPv(pv); 
                    }

                    add_feature(particles[i], zn, Re);	
#if 0
                    cout<<"After multivar gauss"<<endl;
                    cout<<"particle count: "<<particles.size()<<endl;
                    cout<<"w: "<<particles[0].w()<<" " <<"should be 0.0100"<<endl;
                    cout<<"xv: "<<particles[0].xv()<<endl;
                    cout<<"should be 0.7648"<<endl;
                    cout<<"          0.0221"<<endl;
                    cout<<"          0.0050"<<endl;
                    cout<<"Pv: "<<particles[0].Pv()<<" "<<"should be all zeros" <<endl;
                    cout<<"xf: "<<particles[0].xf()<<" "<<"should be empty"<<endl;
                    //cout<<"Pf: "<<particles[0].Pf()[0]<<" "<<"should be empty"<<endl;
                    //cout<<"da: "<<particles[0].da()[0]<<" "<<"should be empty"<<endl;

                    add_feature(particles[i], zn, Re);	

                    cout<<"After add_feature"<<endl;
                    cout<<"particle count: "<<particles.size()<<endl;
                    cout<<"w: "<<particles[0].w()<<" " <<"should be 0.0100"<<endl;
                    cout<<"xv: "<<particles[0].xv()<<endl;
                    cout<<"should be 0.0824"<<endl;
                    cout<<"          0.0036"<<endl;
                    cout<<"          0.0009"<<endl;
                    cout<<"Pv: "<<particles[0].Pv()<<" "<<"should be all zeros" <<endl;
                    cout<<"xf: "<<particles[0].xf()<<endl;
                    cout<<"should be [3.4011 25.7814; -25.6172 3.6347]"<<endl;
                    cout<<"Pf[0]: "<<particles[0].Pf()[0]<<endl;
                    cout<<"should be [0.2004 0.0196; 0.0196 0.0120"<<endl;
                    cout<<"Pf[1]: "<<particles[0].Pf()[1]<<endl;
                    cout<<"should be [0.0138 -0.0261; -0.0261 0.1908"<<endl;
#endif
                }
            }
        }
        delete[] VnGn;
    }
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
