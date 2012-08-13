#include "particle.h"

Particle::Particle() 
{
	x=10;
}

Particle::Particle(const Particle& foo) 
{
	x = foo.x;		
}
