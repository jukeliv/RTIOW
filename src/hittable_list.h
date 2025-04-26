#include "dynamic_array.h"
#include "hittable.h"

#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

typedef struct hittable_list hittable_list;

struct hittable_list {
    hittable* items;
    size_t count;
    size_t capacity;
};

hittable_list create_hittable_list(hittable object) {
    hittable_list list = {0};
    da_append(&list, object);
    return list;
}

void hittable_list_clear(hittable_list* self) {
    self->count = 0;
}

void hittable_list_add(hittable_list* self, hittable object) {
    da_append(self, object);
}

bool hit_hittable_list(hittable* self, const ray r, interval ray_t, hit_record* rec) {
    hittable_list list = *(hittable_list*)self->data;
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    #pragma omp parallel for
    for (size_t i = 0; i < list.count; ++i) {
        hittable object = list.items[i];
        if (object.hit(&object, r, (interval){ray_t.min, closest_so_far}, &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

hittable create_hittable_from_hittable_list(hittable_list* list) {
    return (hittable) {
        .hit = hit_hittable_list,
        .data = list
    };
}

#endif // HITTABLE_LIST_H_