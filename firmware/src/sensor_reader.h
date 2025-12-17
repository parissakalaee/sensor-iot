#ifndef SENSOR_READER_H
#define SENSOR_READER_H

#include <DHT.h>

struct SensorData {
    float temperature;
    float humidity;
    bool isValid;
};

class SensorReader {
public:
    SensorReader(uint8_t pin, uint8_t type);
    void begin();
    SensorData read();

private:
    DHT dht;
};

#endif