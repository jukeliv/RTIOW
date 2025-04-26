#include "mathutil.h"
#include <stdbool.h>
#include <math.h>

#ifndef VEC3_H_
#define VEC3_H_

typedef struct vec3 vec3;

struct vec3 {
    union {
        struct {
            double x;
            double y;
            double z;
        };
        struct {
            double r;
            double g;
            double b;
        };
    };
};

#define create_vec3(_x, _y, _z) ((vec3){.x = (_x), .y = (_y), .z = (_z)})

#define create_random_vec3() (create_vec3(random_double(), random_double(), random_double()))

#define create_random_range_vec3(min, max) (create_vec3(random_double_range((min), (max)), random_double_range((min), (max)), random_double_range((min), (max))))

#define vec3_negate(v3) ((vec3){.x = -(v3).x, .y = -(v3).y, .z = -(v3).z})

// v0 += v1
void vec3_direct_add(vec3* out, vec3 in);

// v0 *= v1
void vec3_direct_mult(vec3* out, vec3 in);

// v0 /= v1
void vec3_direct_d_div(vec3* out, double in);

double vec3_magnitude(vec3 v);
double vec3_magnitude_sqr(vec3 v);

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
typedef vec3 point3;

// Vector Utility Functions
vec3 vec3_add(vec3 u, vec3 v);
vec3 vec3_sub(vec3 u, vec3 v);
vec3 vec3_mult(vec3 u, vec3 v);

vec3 vec3_d_mult(vec3 v, double t);
vec3 vec3_d_div(vec3 v, double t);

double dot(vec3 u, vec3 v);
vec3 cross(vec3 u, vec3 v);

vec3 unit_vector(vec3 v);
vec3 random_unit_vector();

vec3 random_on_hemisphere(vec3 normal);

bool near_zero(vec3 v3);

#define reflect(v, n) (vec3_sub((v), vec3_d_mult((n), 2*dot((v), (n)))))

vec3 refract(vec3 uv, vec3 n, double etai_over_etat);

#endif // VEC3_H_