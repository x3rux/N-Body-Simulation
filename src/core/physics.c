#include "core/physics.h"
#include "core/vec3.h"
#include <math.h>
#include <stddef.h>


void physics_compute_acceleration(Body *bodies, size_t count, float G, float softening){
    for (size_t i = 0; i < count; ++i) {
        bodies[i].acceleration = vec3_zero();
    }

    for (size_t i = 0; i < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            Vec3 r_ij = vec3_sub(bodies[j].position, bodies[i].position);
            float distance_sq = vec3_len_sq(r_ij) + (softening * softening);
            float distance = sqrtf(distance_sq);
            float inv_distance_cube = 1.0f/(distance * distance_sq);

            float factor = G * inv_distance_cube;

            bodies[i].acceleration = vec3_add(bodies[i].acceleration, vec3_scale(r_ij, factor * bodies[j].mass));
            bodies[j].acceleration = vec3_sub(bodies[j].acceleration, vec3_scale(r_ij, factor * bodies[i].mass));
        }
    }

}

void physics_step_leapfrog(Body *bodies, size_t count, float dt, float G, float softening){

    for (size_t i = 0; i < count; ++i) { //half step for velocity, full for position
        bodies[i].velocity = vec3_add(bodies[i].velocity, vec3_scale(bodies[i].acceleration, dt/2));
        bodies[i].position = vec3_add(bodies[i].position, vec3_scale(bodies[i].velocity, dt));
    }

    physics_compute_acceleration(bodies, count, G, softening); // recalculate acceleration

    for (size_t i = 0; i < count; ++i) { //half step for velocity to make full
        bodies[i].velocity = vec3_add(bodies[i].velocity, vec3_scale(bodies[i].acceleration, dt/2));
    }

}
