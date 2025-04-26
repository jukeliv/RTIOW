#include "color.h"
#include "hittable.h"

#ifndef MATERIAL_H_
#define MATERIAL_H_

typedef enum material_kind material_kind;

enum material_kind {
    LambertianMaterial,
    MetalicMaterial,
    DielectricMaterial,
    EmisiveMaterial
};

typedef struct material material;

struct material {
    union {
        struct {
            color albedo;
        } lambertian;
        struct {
            color albedo;
            double fuzz;
        } metalic;
        struct {
            double refraction_index;
        } dielectric;
    };
    material_kind kind;
};

bool scatter_material(material* self, const ray r_in, const hit_record rec, color* attenuation, ray* scattered);

#endif // MATERIAL_H_