#ifndef MQTT_WRAPPER_H
#define MQTT_WRAPPER_H

#include "sensor_reader.h"

#ifdef __cplusplus
extern "C" {
#endif

void mqtt_client_init(const char* broker_uri, const char* topic);
bool mqtt_client_publish_sensor_data(const SensorData& data);
bool mqtt_client_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif // MQTT_WRAPPER_H