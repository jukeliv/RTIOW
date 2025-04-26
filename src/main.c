#include <stdbool.h>
#include <time.h>

#include "mathutil.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "camera.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

// THANKS TSODING FOR ARENA.H AND THE GUYS THAT MADE TIGR.H :D
#define ARENA_IMPLEMENTATION
#include "arena.h"
#include "tigr.h"

static Arena world_arena = {0};

material* make_material(material mat) {
    material* mat_ptr = arena_alloc(&world_arena, sizeof(mat));
    *mat_ptr = mat;
    return mat_ptr;
}

sphere* make_sphere(sphere sp) {
    sphere* sphere_ptr = arena_alloc(&world_arena, sizeof(sp));
    *sphere_ptr = sp;
    return sphere_ptr;
}

hittable make_test_scene(hittable_list* world_list) {
    material* ground_material = make_material((material){
        .kind = LambertianMaterial,
        .lambertian.albedo = create_color(0.5, 0.5, 0.5)
    });

    hittable_list_add(world_list, create_hittable_from_sphere(make_sphere((sphere) {
        .mat = ground_material,
        .center = create_vec3(0, -1000, 0),
        .radius = 1000
    })));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            point3 center = create_vec3(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if (vec3_magnitude(vec3_sub(center, create_vec3(4, 0.2, 0))) > 0.9) {
                material* sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = vec3_mult(random_color(), random_color());
                    sphere_material = make_material((material){.kind = LambertianMaterial, .lambertian.albedo = albedo}); 
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = random_color_range(0.5, 1);
                    double fuzz = random_double_range(0, 0.5);
                    sphere_material = make_material((material){.kind = MetalicMaterial, .metalic.albedo = albedo, .metalic.fuzz = fuzz});
                } else {
                    // glass
                    sphere_material = make_material((material){.kind = DielectricMaterial, .dielectric.refraction_index = 1.5});
                }
                hittable_list_add(world_list, create_hittable_from_sphere(make_sphere((sphere) {
                    .mat = sphere_material,
                    .center = center,
                    .radius = 0.2
                })));
            }
        }
    }

    material* material1 = make_material((material){.kind = DielectricMaterial, .dielectric.refraction_index = 1.5});
    hittable_list_add(world_list, create_hittable_from_sphere(make_sphere((sphere) {
        .mat = material1,
        .center = create_vec3(0, 1, 0),
        .radius = 1.0
    })));

    material* material2 = make_material((material){.kind = LambertianMaterial, .lambertian.albedo = create_color(0.4, 0.2, 0.1)});
    hittable_list_add(world_list, create_hittable_from_sphere(make_sphere((sphere) {
        .mat = material2,
        .center = create_vec3(-4, 1, 0),
        .radius = 1.0
    })));


    material* material3 = make_material((material){.kind = MetalicMaterial, .metalic.albedo = create_color(0.7, 0.6, 0.5)});
    hittable_list_add(world_list, create_hittable_from_sphere(make_sphere((sphere) {
        .mat = material3,
        .center = create_vec3(4, 1, 0),
        .radius = 1.0
    })));

    return create_hittable_from_hittable_list(world_list);
}

int main(int argc, char* argv[]) {
    // World
    srand(time(NULL));
    hittable_list world_list = {0};
    hittable world = make_test_scene(&world_list);

    // Camera
    set_aspect_ratio(16.0 / 9.0);
    
    set_image_width(720);
    set_max_depth(5);
    set_samples_per_pixel(3);

    set_vfov(20);
    set_lookfrom(create_vec3(13,2,3));
    set_lookat(create_vec3(0,0,0));
    set_vup(create_vec3(0,1,0));

    set_defocus_angle(0.6);
    set_focus_distance(10.0);

    // Render
    Tigr* render = NULL;
    bool processing = false;
    bool saved_file = false;
    double render_time;

    tigrTime();
    printf("rendering scene, sorry for not having a graphical display of the rendering :p\n");
    printf("Note: if your render has high fidelity settings, this will take a **WHILE** ( and I mean it )\n");
    render = render_camera(world);
    render_time = tigrTime();
    // Free all the memory after the render is done.
    arena_free(&world_arena);
    da_free(world_list);

    // Try to save the render
    if(argc > 1 && !saved_file) {
        saved_file = true;
        if(tigrSaveImage(argv[1], render) == 0) {
            fprintf(stderr, "%s:%d: error: couldn't save render to file (%s)\n", __FILE__, __LINE__, argv[1]);
        }
    }

    // Window
    Tigr* window = tigrWindow(get_image_width(), get_image_height(), "Raytracing in One Weekend", TIGR_2X);

    while(!tigrClosed(window)) {    
        tigrBlit(window, render, 0, 0, 0, 0, window->w, window->h);
        tigrPrint(window, tfont, 0, 0, tigrRGB(0xff, 0xff, 0xff), "Render took %g seconds", render_time);
    
        tigrUpdate(window);
    }

    tigrFree(window);
    tigrFree(render);
    
    return 0;
}