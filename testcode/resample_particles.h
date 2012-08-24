#ifndef RESAMPLE_PARTICLES_H
#define RESAMPLE_PARTICLES_H

#include <Eigen/Dense>
#include <vector>
#include "particle.h"


using namespace Eigen;
using namespace std;

void resample_particles(vector<Particle> &particles, int Nmin, int doresample); 

#endif //RESAMPLE_PARTICLES_H
