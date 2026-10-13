#include "core/body.h"
#include "core/diagnostics.h"
#include "core/physics.h"
#include "core/vec3.h"
#include <stddef.h>
#include <stdio.h>

#define G 1.0f
#define SOFTENING 0.0f
#define RADIUS 10
#define PI 3.1415926535
#define COUNT 2
#define dt 0.001f

int main(void){

    Body sun = { 0 };
    sun.mass = 1000.0f;

    Body planet = { 0 };
    planet.mass = 1.0f;
    planet.position = vec3_create(RADIUS, 0, 0);
    float orbital_velocity = sqrtf(G * sun.mass / RADIUS);
    planet.velocity = vec3_create(0, orbital_velocity, 0);


    Body bodies[COUNT] = {sun, planet};

    int orbits = 10;
    float period = 2*PI*RADIUS/orbital_velocity;
    size_t steps = (size_t) orbits * period / dt;

    float initial_energy = diagnostic_compute_total_energy(bodies, COUNT, G, SOFTENING);

    physics_compute_acceleration(bodies, COUNT, G, SOFTENING);
    for (size_t i = 0; i < steps; ++i) {
        physics_step_leapfrog(bodies, COUNT, dt, G, SOFTENING);
    }

    float final_energy = diagnostic_compute_total_energy(bodies, COUNT, G, SOFTENING);

    float energy_error = fabsf((final_energy - initial_energy) / initial_energy);
    Vec3 r = vec3_sub(bodies[1].position, bodies[0].position);
    float final_radius = vec3_len(r);
    float radius_error = fabsf((final_radius - RADIUS) / RADIUS);
    printf("=== Kepler 2-Body Orbit Test ===\n");
    printf("Orbits simulated: %d (steps: %zu)\n", orbits, steps);
    printf("Initial Energy:   %.6f\n", initial_energy);
    printf("Final Energy:     %.6f\n", final_energy);
    printf("Energy Drift:     %.6f%%\n", energy_error * 100.0f);
    printf("Final Radius:     %.4f (Expected: %.4f)\n", final_radius, (float)RADIUS);
    if (energy_error < 0.001f && radius_error < 0.01f) {
        printf("[PASS] Stable Keplerian orbit verified!\n");
        return 0;
    } else {
        printf("[FAIL] Orbit unstable or energy drifted!\n");
        return 1;
    }
    return 0;
}
