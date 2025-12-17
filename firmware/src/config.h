#ifndef CONFIG_H
#define CONFIG_H

#include "credentials.h"

// Toggle between brokers
#define USE_HIVEMQ_CLOUD false  // Set to false for public demo

#if USE_HIVEMQ_CLOUD
    #define MQTT_BROKER HIVEMQ_BROKER
    #define MQTT_PORT 8883
    #define MQTT_USER HIVEMQ_USER
    #define MQTT_PASS HIVEMQ_PASS
    #define USE_TLS true
#else
    #define MQTT_BROKER "test.mosquitto.org"
    #define MQTT_PORT 1883
    #define MQTT_USER nullptr
    #define MQTT_PASS nullptr
    #define USE_TLS false
#endif

#define MQTT_TOPIC "parissa/sensors"

#endif