#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFiClient.h>

class MqttClient {
public:
    MqttClient(const char* broker, int port, const char* topic);
    void begin(WiFiClient& wifiClient);
    void loop();
    bool publish(const char* payload);
    bool isConnected();
    
private:
    const char* broker;
    int port;
    const char* topic;
    PubSubClient client;
    
    void reconnect();
    static void callback(char* topic, byte* payload, unsigned int length);
};

#endif