#include <Arduino.h>
#include <ArduinoJson.h>
#include "credentials.h" 
#include "sensor_reader.h"
#include "wifi_manager.h"
#include "mqtt_client.h" 

// MQTT Broker settings
const char* mqtt_broker = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "parissa/sensors";

// DHT11 Sensor settings
#define DHTPIN 4
#define DHTTYPE DHT11

// Initialize objects
SensorReader sensorReader(DHTPIN, DHTTYPE);
WiFiManager wifiManager;
WiFiClient espClient;
MqttClient mqttClient(mqtt_broker, mqtt_port, mqtt_topic); 

// Timing
unsigned long lastPublish = 0;
const long publishInterval = 5000;

void publish_sensor_data() {
  SensorData data = sensorReader.read();

  if (!data.isValid) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  JsonDocument doc;
  doc["temperature"] = data.temperature;
  doc["humidity"] = data.humidity;
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);
  
  if (mqttClient.publish(payload.c_str())) {  // ← Change this
    Serial.print("Published: ");
    Serial.println(payload);
  } else {
    Serial.println("Publish failed!");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== ESP32 IoT Sensor ===");
  
  sensorReader.begin();
  Serial.println("DHT sensor initialized");
  
  wifiManager.connect(WIFI_SSID, WIFI_PASSWORD);

  mqttClient.begin(espClient); 
  
  Serial.println("Setup complete!");
}

void loop() {
  mqttClient.loop();  
  
  unsigned long currentMillis = millis();
  if (currentMillis - lastPublish >= publishInterval) {
    lastPublish = currentMillis;
    publish_sensor_data();
  }
}