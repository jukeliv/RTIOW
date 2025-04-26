#include "vec3.h"

// v0 += v1
void vec3_direct_add(vec3* out, vec3 in) {
    out->x += in.x;
    out->y += in.y;
    out->z += in.z;
}

// v0 *= v1
void vec3_direct_mult(vec3* out, vec3 in) {
    out->x *= in.x;
    out->y *= in.y;
    out->z *= in.z;
}

// v0 /= v1
void vec3_direct_d_div(vec3* out, double in) {
    out->x /= in;
    out->y /= in;
    out->z /= in;
}

double vec3_magnitude(vec3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double vec3_magnitude_sqr(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
typedef vec3 point3;

// Vector Utility Functions

vec3 vec3_add(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x + v.x,
        .y = u.y + v.y,
        .z = u.z + v.z,
    };
}

vec3 vec3_sub(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x - v.x,
        .y = u.y - v.y,
        .z = u.z - v.z,
    };
}

vec3 vec3_mult(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x * v.x,
        .y = u.y * v.y,
        .z = u.z * v.z,
    };
}

vec3 vec3_d_mult(vec3 v, double t) {
    return (vec3) {
        .x = v.x * t,
        .y = v.y * t,
        .z = v.z * t,
    };
}

vec3 vec3_d_div(vec3 v, double t) {
    return (vec3) {
        .x = v.x / t,
        .y = v.y / t,
        .z = v.z / t,
    };
}

double dot(vec3 u, vec3 v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

vec3 cross(vec3 u, vec3 v) {
    return create_vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

vec3 unit_vector(vec3 v) {
    double mag = vec3_magnitude(v);
    return (vec3) {
        .x = v.x / mag,
        .y = v.y / mag,
        .z = v.z / mag,
    };
}

vec3 random_unit_vector(){
    #if 1
    for(;;) {
        vec3 p = create_random_range_vec3(-1,1);
        double lensq = vec3_magnitude_sqr(p);
        if (1e-160 < lensq && lensq <= 1)
            return vec3_d_div(p, sqrt(lensq));
    }
    #else
    // I kinda like constant time for stuff
    return unit_vector(create_random_range_vec3(-1,1));
    #endif
}

vec3 random_on_hemisphere(vec3 normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return vec3_negate(on_unit_sphere);
}

bool near_zero(vec3 v3) {
    // Return true if the vector is close to zero in all dimensions.
    double s = 1e-8;
    return (fabs(v3.x) < s) && (fabs(v3.y) < s) && (fabs(v3.z) < s);
}

vec3 refract(vec3 uv, vec3 n, double etai_over_etat) {
    double cos_theta = MIN(dot(vec3_negate(uv), n), 1.0);
    vec3 r_out_perp =  vec3_d_mult(vec3_add(uv, vec3_d_mult(n, cos_theta)), etai_over_etat);
    vec3 r_out_parallel = vec3_d_mult(n, -sqrt(fabs(1.0 - vec3_magnitude_sqr(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}