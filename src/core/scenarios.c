#include "core/scenarios.h"
#include "core/vec3.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define X 0.97000436f
#define Y -0.24308753f
#define VX 0.46620531f
#define VY 0.43236573f

void scenario_solar_system(Body *bodies, size_t count, float G){
    //sun
    Body sun = {.mass = SUN_MASS};
    bodies[0] = sun;
    float factor = G * sun.mass;

    for (size_t i = 1; i < count; ++i) {
        Body b = { 0 };

        float r = 8.0f + i * 5.0f;
        float v = sqrtf(factor / r);
        float theta = ( (float) rand() / RAND_MAX) * 2.0f * M_PI;

        b.mass = 1.0f + (float) (i % 3);
        b.position = vec3_create(r * cosf(theta), 0, r * sinf(theta));
        b.velocity = vec3_create(-v * sinf(theta), 0, v * cosf(theta));

        bodies[i] = b;
    }
}

void scenario_figure_8(Body *bodies){
    Body b1 = {0}, b2 = {0}, b3 = {0};
    b1.mass = FIGURE_8_MASS;
    b2.mass = FIGURE_8_MASS;
    b3.mass = FIGURE_8_MASS;

    b1.position = vec3_create(X, Y, 0);
    b1.velocity = vec3_create(VX, VY, 0);

    b2.position = vec3_create(-X, -Y, 0);
    b2.velocity = vec3_create(VX, VY, 0);

    b3.position = vec3_create(0, 0, 0);
    b3.velocity = vec3_create(-2*VX, -2*VY, 0);

    bodies[0] = b1;
    bodies[1] = b2;
    bodies[2] = b3;
}
