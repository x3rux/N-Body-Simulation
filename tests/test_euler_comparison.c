#include "core/body.h"
#include "core/diagnostics.h"
#include "core/physics.h"
#include "core/vec3.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#define G 1.0f
#define SOFTENING 0.0f
#define RADIUS 10.0f
#define PI 3.1415926535f
#define COUNT 2

// Standard Explicit/Forward Euler step
static void physics_step_euler_explicit(Body *bodies, size_t count, float dt, float g, float softening) {
    physics_compute_acceleration(bodies, count, g, softening);
    for (size_t i = 0; i < count; ++i) {
        // x(t+dt) = x(t) + v(t)*dt
        // v(t+dt) = v(t) + a(t)*dt
        Vec3 v_old = bodies[i].velocity;
        bodies[i].position = vec3_add(bodies[i].position, vec3_scale(v_old, dt));
        bodies[i].velocity = vec3_add(bodies[i].velocity, vec3_scale(bodies[i].acceleration, dt));
    }
}

int main(void) {
    float orbital_velocity = sqrtf(G * 1000.0f / RADIUS);
    float dt = 0.001f;
    int orbits = 10;
    float period = 2.0f * PI * RADIUS / orbital_velocity;
    size_t steps = (size_t)((orbits * period) / dt);

    // Setup Leapfrog system
    Body lf_bodies[COUNT] = {
        { .mass = 1000.0f, .position = vec3_create(0, 0, 0), .velocity = vec3_create(0, 0, 0) },
        { .mass = 1.0f,    .position = vec3_create(RADIUS, 0, 0), .velocity = vec3_create(0, orbital_velocity, 0) }
    };

    // Setup Forward Euler system (identical starting conditions)
    Body euler_bodies[COUNT] = {
        { .mass = 1000.0f, .position = vec3_create(0, 0, 0), .velocity = vec3_create(0, 0, 0) },
        { .mass = 1.0f,    .position = vec3_create(RADIUS, 0, 0), .velocity = vec3_create(0, orbital_velocity, 0) }
    };

    float e0_lf    = diagnostic_compute_total_energy(lf_bodies, COUNT, G, SOFTENING);
    float e0_euler = diagnostic_compute_total_energy(euler_bodies, COUNT, G, SOFTENING);

    // Initialize accelerations for Leapfrog
    physics_compute_acceleration(lf_bodies, COUNT, G, SOFTENING);

    printf("Simulating %d orbits (%zu steps, dt = %.4f)...\n\n", orbits, steps, dt);

    for (size_t s = 0; s < steps; ++s) {
        physics_step_leapfrog(lf_bodies, COUNT, dt, G, SOFTENING);
        physics_step_euler_explicit(euler_bodies, COUNT, dt, G, SOFTENING);
    }

    float ef_lf    = diagnostic_compute_total_energy(lf_bodies, COUNT, G, SOFTENING);
    float ef_euler = diagnostic_compute_total_energy(euler_bodies, COUNT, G, SOFTENING);

    float err_lf    = fabsf((ef_lf - e0_lf) / e0_lf) * 100.0f;
    float err_euler = fabsf((ef_euler - e0_euler) / e0_euler) * 100.0f;

    float r_lf    = vec3_len(vec3_sub(lf_bodies[1].position, lf_bodies[0].position));
    float r_euler = vec3_len(vec3_sub(euler_bodies[1].position, euler_bodies[0].position));

    printf("===============================================================\n");
    printf("%-20s | %-18s | %-18s\n", "Metric", "Leapfrog (KDK)", "Forward Euler");
    printf("===============================================================\n");
    printf("%-20s | %-18.6f | %-18.6f\n", "Initial Energy", e0_lf, e0_euler);
    printf("%-20s | %-18.6f | %-18.6f\n", "Final Energy", ef_lf, ef_euler);
    printf("%-20s | %-17.6f%% | %-17.6f%%\n", "Energy Drift", err_lf, err_euler);
    printf("%-20s | %-18.4f | %-18.4f\n", "Expected Radius", RADIUS, RADIUS);
    printf("%-20s | %-18.4f | %-18.4f\n", "Final Radius", r_lf, r_euler);
    printf("===============================================================\n");

    return 0;
}
