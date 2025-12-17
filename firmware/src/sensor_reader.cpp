#include "sensor_reader.h"

SensorReader::SensorReader(uint8_t pin, uint8_t type) 
    : dht(pin, type) {
}

void SensorReader::begin() {
    dht.begin();
}

SensorData SensorReader::read() {
    SensorData data;
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();
    
    // Validate readings
    data.isValid = !isnan(data.temperature) && 
                   !isnan(data.humidity) &&
                   data.temperature > -40 && data.temperature < 80 &&
                   data.humidity > 0 && data.humidity < 100;
    
    return data;
}