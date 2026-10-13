#include "core/diagnostics.h"
#include "core/vec3.h"
#include <stddef.h>

float diagnostic_compute_kinetic_energy(const Body *bodies, size_t count){
    float result = 0.0f;

    for (size_t i = 0; i < count; ++i) {
        result += 0.5f * bodies[i].mass * vec3_len_sq(bodies[i].velocity);
    }

    return result;
}

float diagnostic_compute_potential_energy(const Body *bodies, size_t count, float G, float softening){
    float result = 0.0f;

    //calculate pair-wise, using symmetry like in acceleration calculation
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            Vec3 r_ij = vec3_sub(bodies[j].position, bodies[i].position);
            float distance = vec3_len_sq(r_ij) + (softening * softening);
            float inverse_distance_sqrt = 1.0f/sqrtf(distance);

            //pairwise energy
            result += (-G) * bodies[i].mass * bodies[j].mass * inverse_distance_sqrt;
        }
    }

    return result;
}

float diagnostic_compute_total_energy(const Body *bodies, size_t count, float G, float softening){
    float result;

    result = diagnostic_compute_kinetic_energy(bodies, count) + diagnostic_compute_potential_energy(bodies, count, G, softening);

    return result;
}
