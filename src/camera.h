#include <omp.h>

#include "mathutil.h"
#include "material.h"
#include "hittable.h"
#include "color.h"

#include "tigr.h"

#ifndef CAMERA_H_
#define CAMERA_H_

void set_lookfrom(vec3 v);
void set_lookat(vec3 v);
void set_vup(vec3 v);

Tigr* render_camera(hittable world);
void initialize_camera();

int get_image_width();
int get_image_height();

void set_lookfrom(vec3 v);
void set_lookat(vec3 v);
void set_vup(vec3 v);
void set_aspect_ratio(double v);
void set_image_width(int v);
void set_vfov(double v);
void set_samples_per_pixel(int v);
void set_max_depth(int v);
void set_defocus_angle(double v);
void set_focus_distance(double v);

#endif // CAMERA_H_