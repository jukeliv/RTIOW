#include "camera.h"

// public:
double  aspect_ratio = 1.0;             // Ratio of image width over height
int     image_width = 100;              // Rendered image width in pixel count
int     samples_per_pixel = 10;         // Count of random samples for each pixel
int     max_depth = 10;

double  vfov = 90;                      // Vertical view angle ( field of view )
point3 lookfrom = create_vec3(0,0,0);   // Point camera is looking from
point3 lookat   = create_vec3(0,0,-1);  // Point camera is looking at
vec3   vup      = create_vec3(0,1,0);   // Camera-relative "up" direction

double defocus_angle = 0;               // Variation angle of rays through each pixel
double focus_distance = 10;             // Distance from camera lookfrom point to plane of perfect focus

// private:
int     image_height;                   // Rendered image height
double  pixel_samples_scale;
point3  center;                         // Camera center
point3  pixel00_loc;                    // Location of pixel 0, 0
vec3    pixel_delta_u;                  // Offset to pixel to the right
vec3    pixel_delta_v;                  // Offset to pixel below
vec3   defocus_disk_u;                  // Defocus disk horizontal radius
vec3   defocus_disk_v;                  // Defocus disk vertical radius

int get_image_width() {
    return image_width;
}

int get_image_height() {
    return MAX(1, (int)(image_width / aspect_ratio));
}

void initialize_camera() {
    max_depth = max_depth;

    samples_per_pixel = samples_per_pixel;
    pixel_samples_scale = 1.0 / samples_per_pixel;

    image_width = image_width;
    image_height = (int)(image_width / aspect_ratio);
    image_height = MAX(1, image_height);

    center = lookfrom;

    // Determine viewport dimensions.
    // double focal_length = vec3_magnitude(vec3_sub(lookfrom, lookat));
    double theta = degrees_to_radians(vfov);
    double h = tan(theta/2);
    double viewport_height = 2 * h * focus_distance;
    double viewport_width = viewport_height * ((double)(image_width)/image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    vec3 w = unit_vector(vec3_sub(lookfrom, lookat));
    vec3 u = unit_vector(cross(vup, w));
    vec3 v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = vec3_d_mult(u, viewport_width);    // Vector across viewport horizontal edge
    vec3 viewport_v = vec3_d_mult(vec3_negate(v), viewport_height);  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = vec3_d_div(viewport_u, image_width);
    pixel_delta_v = vec3_d_div(viewport_v, image_height);

    // Calculate the location of the upper left pixel.
    vec3 viewport_upper_left = vec3_sub(vec3_sub(vec3_sub(center, vec3_d_mult(w, focus_distance)), vec3_d_div(viewport_u, 2)), vec3_d_div(viewport_v, 2));
    pixel00_loc = vec3_add(viewport_upper_left, vec3_d_mult(vec3_add(pixel_delta_u, pixel_delta_v), 0.5));

    // Calculate the camera defocus disk basis vectors.
    double defocus_radius = focus_distance * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = vec3_d_mult(u, defocus_radius);
    defocus_disk_v = vec3_d_mult(v, defocus_radius);
}

vec3 random_in_unit_disk() {
    #if 1
    for(;;) {
        vec3 p = create_vec3(random_double_range(-1,1), random_double_range(-1,1), 0);
        if (vec3_magnitude_sqr(p) < 1)
            return p;
    }
    #else
    return unit_vector(create_vec3(random_double_range(-1,1), random_double_range(-1,1), 0));
    #endif
}

point3 defocus_disk_sample() {
    // Returns a random point in the camera defocus disk.
    vec3 p = random_in_unit_disk();
    return vec3_add(vec3_add(center, vec3_d_mult(defocus_disk_u, p.x)), vec3_d_mult(defocus_disk_v, p.y));
}

ray get_ray(int i, int j) {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j.

    vec3 offset = create_vec3(random_double() - 0.5, random_double() - 0.5, 0); // sample_square();
    vec3 pixel_sample = vec3_add(vec3_add(pixel00_loc, vec3_d_mult(pixel_delta_u, (i + offset.x))), vec3_d_mult(pixel_delta_v, (j + offset.y)));

    vec3 ray_origin;
    if(defocus_angle <= 0) {
        ray_origin = center;
     } else {
        ray_origin = defocus_disk_sample();
     }
    vec3 ray_direction = vec3_sub(pixel_sample, ray_origin);

    return create_ray(ray_origin, ray_direction);
}

color ray_color(ray r, int depth, hittable world) {
    if(depth <= 0)
        return create_color(0,0,0);

    hit_record rec;

    if(world.hit(&world, r, (interval){0.001, INFINITY}, &rec)) {
        ray scattered;
        color attenuation;
        if (scatter_material(rec.mat, r, rec, &attenuation, &scattered))
            return vec3_mult(attenuation, ray_color(scattered, depth-1, world));
        return create_color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.dir);
    double a = (unit_direction.y + 1.0)/2;
    double inv_a = 1.0 - a;
    return vec3_add(create_color(inv_a, inv_a, inv_a), create_color(0.5*a, 0.7*a, 1.0*a));
}

Tigr* render_camera(hittable world) {
    initialize_camera();

    // double image_height_inverse = 100.0/image_height;

    Tigr* render = tigrBitmap(image_width, image_height);
    
    const size_t n_pixels = image_width*image_height;

    #pragma omp target teams distribute parallel for collapse(2) \
                map(to:render->pix) map(from:render->pix[0:n_pixels])
    for(int j = 0; j < image_height; ++j) {
        // printf("%g%% done\n", (j+1)*image_height_inverse);
        for(int i = 0; i < image_width; ++i) {
            color pixel_color = {0};
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                pixel_color = vec3_add(pixel_color, ray_color(r, max_depth, world));
            }

            render->pix[j * image_width + i] = color_to_pixel(vec3_d_mult(pixel_color, pixel_samples_scale));
        }
    }

    return render;
}

// CAMERA CONFIGURATION
void set_lookfrom(vec3 v) {
    lookfrom = v;
}

void set_lookat(vec3 v) {
    lookat = v;
}

void set_vup(vec3 v) {
    vup = v;
}

void set_aspect_ratio(double v) {
    aspect_ratio = v;
}

void set_image_width(int v) {
    image_width = v;
}

void set_vfov(double v) {
    vfov = v;
}

void set_samples_per_pixel(int v) {
    samples_per_pixel = v;
}

void set_max_depth(int v) {
    max_depth = v;
}

void set_defocus_angle(double v) {
    defocus_angle = v;
}

void set_focus_distance(double v) {
    focus_distance = v;
}