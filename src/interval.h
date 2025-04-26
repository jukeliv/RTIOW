#include <math.h>

#ifndef INTERVAL_H_
#define INTERVAL_H_

typedef struct interval interval;

struct interval {
    double min, max;
};

#define interval_size(interval) ((interval).max - (interval).min)

#define interval_contains(interval, x) ((interval).min <= (x) && (x) <= (interval).max)

#define interval_surrounds(interval, x) ((interval).min < (x) && (x) < (interval).max)

#define interval_clamp(interval, x) (CLAMP((x), (interval).min, (interval).max))

#define EMTPY_INTERVAL      ((interval){.min = INFINITY, .max = -INFINITY})
#define UNIVERSE_INTERVAL   ((interval){.min = -INFINITY, .max = INFINITY})

#endif // INTERVAL_H_