#include "hittable.h"

void hit_record_set_face_normal(hit_record* self, const ray r, const vec3 outward_normal) {
    self->front_face = dot(r.dir, outward_normal) < 0;
    self->normal = self->front_face ? outward_normal : vec3_negate(outward_normal);
}