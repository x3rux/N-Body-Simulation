#include "core/diagnostics.h"
#include "core/vec3.h"
#include "render/renderer.h"
#include  "core/physics.h"
#include <math.h>
#include <stdbool.h>
#include <raylib.h>
#include <stddef.h>

#define HEIGHT 600
#define WIDTH 960
#define BOX_SIZE 10.0f
#define G 1.0f
#define SOFTENING 0.0f
#define COUNT 4

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "N-Body Simulator");
    SetTargetFPS(60);
    Camera3D camera = renderer_init_camera();

    Body sun = { .mass = 1000.0f };
    float factor = G * sun.mass;

    Body a = { .mass = 1.0f , .position = vec3_create(8, 0, 0) };
    float vel_a = sqrtf(factor / a.position.x);
    a.velocity.z = vel_a;

    Body b = { .mass = 2.0f, .position = vec3_create(15, 0, 0) };
    float vel_b = sqrtf(factor / b.position.x);
    b.velocity.z = vel_b;

    Body c = { .mass = 0.5f, .position = vec3_create(24, 0, 0) };
    float vel_c = sqrtf(factor / c.position.x);
    c.velocity.z = vel_c;

    Body bodies[COUNT] = {sun, a, b, c};

    physics_compute_acceleration(bodies, COUNT, G, SOFTENING);
    float initial_energy = diagnostic_compute_total_energy(bodies, COUNT, G, SOFTENING);

    bool is_paused = false;
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            is_paused = !is_paused;
        }

        if (!is_paused) {
            float dt = 0.01f;
            int substeps = 4;
            for (int i = 0; i < substeps; ++i) { //for each frame, we break 0.01 into 4 small substeps
                physics_step_leapfrog(bodies, COUNT, dt / substeps, G, SOFTENING);
            }
        }

        float current_energy = diagnostic_compute_total_energy(bodies, COUNT, G, SOFTENING);

        float energy_error = fabsf((current_energy - initial_energy) / initial_energy);
        float energy_error_pct = energy_error * 100.0f;

        renderer_update_camera(&camera);

        BeginDrawing();
        ClearBackground(BLACK);

        renderer_draw_scene(&camera, bodies, COUNT);
        renderer_draw_hud(COUNT, current_energy, energy_error_pct, is_paused);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
