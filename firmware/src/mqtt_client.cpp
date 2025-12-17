#include "mqtt_client.h"

MqttClient::MqttClient(const char* broker, int port, const char* topic)
    : broker(broker), port(port), topic(topic) {
}

void MqttClient::begin(WiFiClient& wifiClient) {
    client.setClient(wifiClient);
    client.setServer(broker, port);
    client.setCallback(callback);
}

void MqttClient::loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

bool MqttClient::publish(const char* payload) {
    return client.publish(topic, payload);
}

bool MqttClient::isConnected() {
    return client.connected();
}

void MqttClient::reconnect() {
    // NO while loop! Try once per call
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" will retry next loop");
        }
    }
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}