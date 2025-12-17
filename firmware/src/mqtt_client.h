#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFiClient.h>

class MqttClient {
public:
    MqttClient(const char* broker, int port, const char* topic);
    void begin(Client& client, const char* user = nullptr, const char* pass = nullptr); 
    void loop();
    bool publish(const char* payload);
    bool isConnected();
    
private:
    const char* broker;
    int port;
    const char* topic;
    const char* username;  
    const char* password;  
    PubSubClient client;
    
    void reconnect();
    static void callback(char* topic, byte* payload, unsigned int length);
};

#endif