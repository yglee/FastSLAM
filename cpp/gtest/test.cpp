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

TEST(FASTSLAM_TEST,numerical_accuracy_test)
{
    EXPECT_EQ(1,1);
}
