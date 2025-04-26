#include <math.h>
#include <stdlib.h>

#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))

#define CLAMP(x, min, max) (MIN(MAX((x), (min)), (max)))

#define degrees_to_radians(degrees) ((degrees) * M_PI / 180.0)

#define random_double() (rand() / (RAND_MAX + 1.0))

#define random_double_range(min, max) ((min) + ((max)-(min))*random_double())

#endif // MATH_UTIL_H_