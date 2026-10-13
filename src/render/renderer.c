#include "render/renderer.h"
#include <math.h>
#include <raylib.h>
#include <stddef.h>

Camera3D renderer_init_camera(void){
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 25.0f, 40.0f };
    camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}

void renderer_draw_scene(const Camera3D *camera, const Body *bodies, size_t count, bool draw_grid, float base_radius){
    BeginMode3D(*camera);

    if (draw_grid) {
        DrawGrid(20, 2.0f);
    }

    const Color palette[] = { SKYBLUE, RED, GREEN, ORANGE, PURPLE, PINK, YELLOW };
    size_t palette_len = sizeof(palette) / sizeof(palette[0]);

    for (size_t i = 0; i < count; ++i){
        float radius = (bodies[i].mass >= STAR_MASS_THRESHOLD)
                       ? 1.5f
                       : fmaxf(base_radius, base_radius * cbrtf(bodies[i].mass));
        Vector3 center = (Vector3) {bodies[i].position.x, bodies[i].position.y, bodies[i].position.z};
        Color color = (bodies[i].mass >= STAR_MASS_THRESHOLD) ? GOLD : palette[i % palette_len];

        DrawSphere(center, radius, color);
    }

    EndMode3D();
}

void renderer_draw_hud(size_t body_count, float total_energy, float energy_drift_pct, bool is_paused){
    DrawFPS(20, 20);
    DrawText(TextFormat("Bodies: %zu", body_count), 20, 45, 18, RAYWHITE);
    DrawText(TextFormat("Energy: %.4f", total_energy), 20, 70, 18, RAYWHITE);
    DrawText(TextFormat("Drift:  %.4f%%", energy_drift_pct), 20, 95, 18,
             (fabsf(energy_drift_pct) < 0.1f) ? GREEN : RED);

    if (is_paused) {
        DrawText("PAUSED (Space to Resume)", 20, 125, 20, YELLOW);
    }
}

void renderer_update_camera(Camera3D *camera){
    UpdateCamera(camera, CAMERA_ORBITAL);
}
