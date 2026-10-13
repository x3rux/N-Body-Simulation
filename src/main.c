#include "core/body.h"
#include "core/diagnostics.h"
#include "core/physics.h"
#include "core/scenarios.h"
#include "core/vec3.h"
#include "render/renderer.h"

#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>

#define WIDTH 1280
#define HEIGHT 720
#define MAX_BODIES 256
#define SUBSTEPS 4

static void load_scenario(ScenarioType type, Body *bodies, size_t *out_count,
                          float *out_softening, float *out_dt, float G, Camera3D *camera) {
    switch (type) {
        case SCENARIO_FIGURE_8:
            *out_count = 3;
            *out_softening = 0.0f;       // Zero softening for exact figure-8 choreography
            *out_dt = 0.008f;           // Snappy speed for Figure-8 loop
            scenario_figure_8(bodies);

            // Face-on camera locked on the X-Y plane
            camera->position = (Vector3){ 0.0f, 0.0f, 3.0f };
            camera->target   = (Vector3){ 0.0f, 0.0f, 0.0f };
            camera->up       = (Vector3){ 0.0f, 1.0f, 0.0f };
            break;

        case SCENARIO_SOLAR_SYSTEM:
        default:
            *out_count = 12;
            *out_softening = 0.05f;
            *out_dt = 0.01f;
            scenario_solar_system(bodies, *out_count, G);

            // Elevated camera view overlooking the planetary disk
            camera->position = (Vector3){ 0.0f, 35.0f, 50.0f };
            camera->target   = (Vector3){ 0.0f, 0.0f, 0.0f };
            camera->up       = (Vector3){ 0.0f, 1.0f, 0.0f };
            break;
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "N-Body Simulation");
    SetTargetFPS(60);

    Camera3D camera = renderer_init_camera();

    Body bodies[MAX_BODIES] = { 0 };
    size_t count = 0;
    float G = 1.0f;
    float softening = 0.0f;
    float dt = 0.01f;

    ScenarioType current_scenario = SCENARIO_SOLAR_SYSTEM;
    load_scenario(current_scenario, bodies, &count, &softening, &dt, G, &camera);

    physics_compute_acceleration(bodies, count, G, softening);
    float initial_energy = diagnostic_compute_total_energy(bodies, count, G, softening);

    bool is_paused = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            is_paused = !is_paused;
        }

        if (IsKeyPressed(KEY_ONE)) {
            current_scenario = SCENARIO_SOLAR_SYSTEM;
            load_scenario(current_scenario, bodies, &count, &softening, &dt, G, &camera);
            physics_compute_acceleration(bodies, count, G, softening);
            initial_energy = diagnostic_compute_total_energy(bodies, count, G, softening);
        }

        if (IsKeyPressed(KEY_TWO)) {
            current_scenario = SCENARIO_FIGURE_8;
            load_scenario(current_scenario, bodies, &count, &softening, &dt, G, &camera);
            physics_compute_acceleration(bodies, count, G, softening);
            initial_energy = diagnostic_compute_total_energy(bodies, count, G, softening);
        }

        // Speed control (time warp)
        if (IsKeyPressed(KEY_UP)) {
            dt *= 1.5f;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            dt /= 1.5f;
        }

        //reset
        if (IsKeyPressed(KEY_R)) {
            load_scenario(current_scenario, bodies, &count, &softening, &dt, G, &camera);
            physics_compute_acceleration(bodies, count, G, softening);
            initial_energy = diagnostic_compute_total_energy(bodies, count, G, softening);
        }

        if (!is_paused) {
            float sub_dt = dt / (float)SUBSTEPS;
            for (int s = 0; s < SUBSTEPS; ++s) {
                physics_step_leapfrog(bodies, count, sub_dt, G, softening);
            }
        }

        float current_energy = diagnostic_compute_total_energy(bodies, count, G, softening);
        float energy_error_pct = fabsf((current_energy - initial_energy) / initial_energy) * 100.0f;

        // Only allow mouse orbit in Solar System; keep Figure-8 camera locked face-on
        if (current_scenario == SCENARIO_SOLAR_SYSTEM) {
            renderer_update_camera(&camera);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        bool draw_grid = (current_scenario == SCENARIO_SOLAR_SYSTEM);
        float base_radius = (current_scenario == SCENARIO_FIGURE_8) ? 0.07f : 0.45f;
        renderer_draw_scene(&camera, bodies, count, draw_grid, base_radius);
        renderer_draw_hud(count, current_energy, energy_error_pct, is_paused);

        const char *scenario_name = (current_scenario == SCENARIO_FIGURE_8) ? "Figure-8 3-Body" : "Solar System";
        DrawText(TextFormat("Scenario: %s  |  [1] Solar  [2] Fig-8  [Up/Down] Speed (%.3fx)  [R] Reset  [Space] Pause",
                            scenario_name, dt / 0.01f),
                 20, HEIGHT - 30, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
