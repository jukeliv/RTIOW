#include <stdlib.h>
#include <assert.h>

#ifndef DA_H_
#define DA_H_

#ifndef DA_INIT_CAP
#define DA_INIT_CAP 256
#endif

#define da_reserve(da, expected_capacity)                                              \
    do {                                                                                   \
        if ((expected_capacity) > (da)->capacity) {                                        \
            if ((da)->capacity == 0) {                                                     \
                (da)->capacity = DA_INIT_CAP;                                          \
            }                                                                              \
            while ((expected_capacity) > (da)->capacity) {                                 \
                (da)->capacity *= 2;                                                       \
            }                                                                              \
            (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            assert((da)->items != (void*)(0) && "Buy more RAM lol");                         \
        }                                                                                  \
    } while (0)

// Append an item to a dynamic array
#define da_append(da, item)                \
    do {                                       \
        da_reserve((da), (da)->count + 1); \
        (da)->items[(da)->count++] = (item);   \
    } while (0)

#define da_free(da) free((da).items)

#endif // DA_H_