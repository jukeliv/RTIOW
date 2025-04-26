#include "material.h"

static double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

bool scatter_material(material* self, const ray r_in, const hit_record rec, color* attenuation, ray* scattered) {
    switch(self->kind) {
        case LambertianMaterial: {
            vec3 scatter_direction = vec3_add(rec.normal, random_unit_vector());
            
            if(near_zero(scatter_direction))
                scatter_direction = rec.normal;

            *scattered = create_ray(rec.p, scatter_direction);
            *attenuation = self->lambertian.albedo;

            return true;
        } break;

        case MetalicMaterial: {
            vec3 reflected = reflect(r_in.dir, rec.normal);
            reflected = vec3_add(unit_vector(reflected), vec3_d_mult(random_unit_vector(), self->metalic.fuzz));
            *scattered = create_ray(rec.p, reflected);
            *attenuation = self->metalic.albedo;

            return (dot(scattered->dir, rec.normal) > 0);
        } break;

        case DielectricMaterial: {
            *attenuation = create_color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/self->dielectric.refraction_index) : self->dielectric.refraction_index;

            vec3 unit_direction = unit_vector(r_in.dir);
            double cos_theta = MIN(dot(vec3_negate(unit_direction), rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);

            *scattered = create_ray(rec.p, direction);
            
            return true;
        } break;
    }

    return false;
}