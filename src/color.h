#include "tigr.h"
#include "vec3.h"
#include "mathutil.h"

#ifndef COLOR_H_
#define COLOR_H_

typedef vec3 color;

#define create_color(_r, _g, _b) ((vec3){.r = (_r), .g = (_g), .b = (_b)})

// TODO: Maybe just add tone mapping, like 1/x tone mapping or sum
// #define TONE_MAPPER(x) ((int)(256 * CLAMP((x), 0.000, 0.999)))   // LINEAR RGB CLAMP
// #define TONE_MAPPER(x) (int)(((x)*(1 + (x)/1.5))/(1+(x)) * 255)  // BASIC REINHARD EXTENDED
// #define TONE_MAPPER(x) ((int)(256 * CLAMP(((x)*(2.51*(x)+0.03))/((x)*(2.43*(x)+0.59)+0.14), 0.000, 0.999))) // NARKOWICZ ACES
#define TONE_MAPPER(x) ((int)(256 * CLAMP((x) > 0 ? sqrt(x) : 0, 0.000, 0.999))) // LINEAR TO GAMMA RGB CLAMP

#define color_to_pixel(c) ((TPixel){ \
    .r = TONE_MAPPER((c).r), \
    .g = TONE_MAPPER((c).g), \
    .b = TONE_MAPPER((c).b), \
    .a = 0xff})

#define random_color() create_color(random_double(), random_double(), random_double())
#define random_color_range(min, max) create_color(random_double_range(min, max), random_double_range(min, max), random_double_range(min, max))

#endif // COLOR_H_