#include "fastslam1/fastslam1_sim.h"
#include "core/add_control_noise.h"
#include "core/get_observations.h"
#include "core/add_observation_noise.h"
#include "core/TransformToGlobal.h"
#include "core/line_plot_conversion.h"
#include "core/data_associate_known.h"
#include "core/feature_update.h"
#include "core/resample_particles.h"
#include "core/add_feature.h"
#include "fastslam1/compute_weight.h"
#include "fastslam1/predict.h"
#include <gtest/gtest.h>
#include "core/read_input_file.h"

TEST(FASTSLAM_TEST,compute_steering_test)
{
    MatrixXf lm; //landmark positions
	MatrixXf wp; //way points
    
	read_input_file("example_webmap.mat", &lm, &wp);
    
    VectorXf xtrue(3);
    xtrue.setZero();
    
    int iwp =0;
    float minD = 1;
    float G = 0;
    float rateG = 0.3491;
    float maxG = 0.5236;
    float dt = 0.0250;

    compute_steering(xtrue,wp,iwp,minD,G,rateG,maxG,dt);
    
    EXPECT_NE(G,-0.0087);
    EXPECT_EQ(iwp,0);
}

TEST(FASTSLAM_TEST,predict_true_test)
{
    VectorXf xtrue(3);
    xtrue.setZero();
    
    float V = 3;
    float G = -0.0087;
    int WB = 4;
    float dt = 0.0250;
    
    predict_true(xtrue,V,G,WB,dt);
    
    VectorXf xtrue_out(3);
    xtrue_out<<0.0750,-0.0007,-0.0002;
    
    EXPECT_NE(xtrue,xtrue_out); 
}

TEST(FASTSLAM_TEST,add_control_noise_test) {
    float V = 3;
    float G = -0.0087;
    Matrix2f Q;
    Q   <<0.0900,0,
    0,0.0027;
    int SWITCH_CONTROL_NOISE = 1;
    float* VnGn = new float[2];
    add_control_noise(V,G,Q,SWITCH_CONTROL_NOISE,VnGn);
    
    float Vn = 2.6056;
    float Gn = -0.0305;
    EXPECT_NE(VnGn[0],Vn);
    EXPECT_NE(VnGn[1],Gn);
}
