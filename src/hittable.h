#include <stdbool.h>

#include "ray.h"
#include "interval.h"

#ifndef HITTABLE_H_
#define HITTABLE_H_

typedef struct hit_record hit_record;

struct hit_record {
    point3 p;
    vec3 normal;
    struct material* mat;
    double t;
    bool front_face;
};

typedef struct hittable hittable;

struct hittable {
    bool(*hit)(hittable* self, const ray r, interval ray_t, hit_record* rec);
    void* data;
};

void hit_record_set_face_normal(hit_record* self, const ray r, const vec3 outward_normal);

#endif // HITTABLE_H_