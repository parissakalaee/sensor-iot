#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"

#if USE_TLS
  #include <WiFiClientSecure.h>
  WiFiClientSecure espClient;
#else
  #include <WiFiClient.h>
  WiFiClient espClient;
#endif

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
  
  #if USE_TLS
    Serial.println("=== ESP32 IoT Sensor (TLS) ===");
  #else
    Serial.println("=== ESP32 IoT Sensor ===");
  #endif
  
  sensorReader.begin();
  Serial.println("DHT sensor initialized");
  
  wifiManager.connect(WIFI_SSID, WIFI_PASSWORD);

  #if USE_TLS
    Serial.print("Syncing time...");
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    // Wait for time sync (max 10 seconds)
    int timeout = 20;
    while (time(nullptr) < 8 * 3600 * 2 && timeout-- > 0) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(timeout > 0 ? " OK!" : " TIMEOUT!");

    espClient.setInsecure();
  #endif

  mqttClient.begin(espClient, MQTT_USER, MQTT_PASS);
  
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