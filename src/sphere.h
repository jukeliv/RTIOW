#include "mathutil.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"

#ifndef SPHERE_H_
#define SPHERE_H_

typedef struct sphere sphere;

struct sphere {
    material* mat;
    point3 center;
    double radius;
};

sphere create_sphere(material* mat, point3 center, double radius) {
    return (sphere){.mat = mat, .center = center, .radius = MAX(0, radius)};
}

bool hit_sphere(hittable* self, const ray r, interval ray_t, hit_record* rec) {
    const sphere object = *(sphere*)self->data;
    vec3 oc = vec3_sub(object.center, r.origin);
    double a = vec3_magnitude_sqr(r.dir);
    double h = dot(r.dir, oc);
    double c = vec3_magnitude_sqr(oc) - object.radius*object.radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (!interval_surrounds(ray_t, root)) {
        root = (h + sqrtd) / a;
        if (!interval_surrounds(ray_t, root))
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    vec3 outward_normal = vec3_d_div(vec3_sub(rec->p, object.center), object.radius);
    hit_record_set_face_normal(rec, r, outward_normal);
    rec->mat = object.mat;

    return true;
}

hittable create_hittable_from_sphere(sphere* sphere) {
    return (hittable) {
        .hit = hit_sphere,
        .data = sphere
    };
}

#endif // SPHERE_H_