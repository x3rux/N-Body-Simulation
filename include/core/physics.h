#ifndef NBODY_PHYSICS_H
#define NBODY_PHYSICS_H
#include "body.h"
#include <stddef.h>

// computes acceleration for each body in bodies with plummer softening
void physics_compute_acceleration(Body *bodies, size_t count, float G, float softening);

// leap frog integrator for veloity, position and acceleration
void physics_step_leapfrog(Body *bodies, size_t count, float dt, float G, float softening);

#endif
