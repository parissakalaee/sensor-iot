// firmware-espidf/main/sensor_reader.h
#ifndef SENSOR_READER_H
#define SENSOR_READER_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float temperature;
    float humidity;
    bool isValid;
} SensorData;

void sensor_reader_init(gpio_num_t gpio_pin);
SensorData sensor_reader_read(void);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_READER_H