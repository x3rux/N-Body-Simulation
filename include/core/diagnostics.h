#ifndef NBODY_DIAGNOSTICS_H
#define NBODY_DIAGNOSTICS_H
#include "core/body.h"
#include <stddef.h>

float diagnostic_compute_kinetic_energy(const Body *bodies, size_t count);

float diagnostic_compute_potential_energy(const Body *bodies, size_t count, float G, float softening);

float diagnostic_compute_total_energy(const Body *bodies, size_t count, float G, float softening);

#endif
