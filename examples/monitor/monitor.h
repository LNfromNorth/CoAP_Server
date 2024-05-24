#ifndef __MONITOR_H
#define __MONITOR_H

#include <stdint.h>

typedef struct sensor_t {
    int device;
    int temperature;
    int humidity;
}sensor_t;

#endif
