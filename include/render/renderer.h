#ifndef NBODY_RENDERER_H
#define NBODY_RENDERER_H
#define STAR_MASS_THRESHOLD 100.0f

#include "core/body.h"
#include <raylib.h>
#include <stddef.h>

Camera3D renderer_init_camera(void);

void renderer_update_camera(Camera3D *camera);

void renderer_draw_scene(const Camera3D *camera, const Body *bodies, size_t count, bool draw_grid, float base_radius);

void renderer_draw_hud(size_t body_count, float total_energy, float energy_drift_pct, bool is_paused);

#endif
