#ifndef NBODY_BODY_H
#define NBODY_BODY_H
#include "vec3.h"

typedef struct{
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    float mass;
} Body;


#endif
