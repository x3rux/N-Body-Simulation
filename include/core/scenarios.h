#ifndef NBODY_SCENARIOS_H
#define NBODY_SCENARIOS_H
#include "core/body.h"
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define SUN_MASS 1000.0f
#define FIGURE_8_MASS 1.0f

typedef enum{
    SCENARIO_SOLAR_SYSTEM,
    SCENARIO_FIGURE_8,
    SCENARIO_GALAXY_DISK
} ScenarioType;

void scenario_solar_system(Body *bodies, size_t count, float G);

void scenario_figure_8(Body *bodies);

void scenario_galaxy_disk(Body *bodies, size_t count, float G);

#endif
