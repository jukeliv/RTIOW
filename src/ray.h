#include "vec3.h"

#ifndef RAY_H_
#define RAY_H_

typedef struct ray ray;

struct ray {
    point3 origin;
    vec3 dir;
};

#define create_ray(_origin, _direction) ((ray){.origin = (_origin), .dir = (_direction)})

#define ray_at(r, t) (vec3_add((r).origin, vec3_d_mult((r).dir, (t))))

#endif