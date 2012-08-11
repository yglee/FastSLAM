#ifndef RESAMPLE_PARTICLES_H
#define RESAMPLE_PARTICLES_H

#include <Eigen/Dense>

using namespace Eigen;

void resample_particles(Particle *particles, int Nmin, int doresample); 

#endif //RESAMPLE_PARTICLES_H
