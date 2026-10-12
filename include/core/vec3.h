#ifndef NBODY_VEC3_H
#include <math.h>
#define NBODY_VEC3_H

typedef struct{
    float x;
    float y;
    float z;
} Vec3;

static inline Vec3 vec3_create(float x, float y, float z){
    return (Vec3) {x, y, z};
}

static inline Vec3 vec3_add(Vec3 a, Vec3 b){
    return (Vec3) {a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b){
    return (Vec3) {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3 vec3_cross(Vec3 a, Vec3 b){
    return (Vec3) {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

static inline float vec3_dot(Vec3 a, Vec3 b){
    return (float) (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

static inline float vec3_len_sq(Vec3 a){
    return vec3_dot(a, a);
}

static inline float vec3_len(Vec3 a){
    return sqrtf(vec3_len_sq(a));
}

static inline Vec3 vec3_scale(Vec3 a, float scale){
    return (Vec3) {a.x * scale, a.y * scale, a.z * scale};
}


#endif
