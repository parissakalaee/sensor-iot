#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "credentials.h" 
#include "sensor_reader.h"
#include "wifi_manager.h"


// MQTT Broker settings
const char* mqtt_broker = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "parissa/sensors";

// DHT11 Sensor settings
#define DHTPIN 4        // GPIO4 (D4)
#define DHTTYPE DHT11   // DHT11 sensor

// Initialize objects
SensorReader sensorReader(DHTPIN, DHTTYPE);
WiFiManager wifiManager;
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Timing
unsigned long lastPublish = 0;
const long publishInterval = 5000; // 5 seconds

void reconnect_mqtt() {
  while (!mqtt_client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    
    // Create a unique client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("Connected!");
      // Subscribe to the same topic to verify round-trip
      mqtt_client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

void publish_sensor_data() {
  // Read sensor data
  SensorData data = sensorReader.read();

  // Check if readings are valid
  if (!data.isValid) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create JSON payload
  JsonDocument doc;
  doc["temperature"] = data.temperature;
  doc["humidity"] = data.humidity;
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);
  
  // Publish to MQTT
  if (mqtt_client.publish(mqtt_topic, payload.c_str())) {
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
  
  // Initialize DHT sensor
  sensorReader.begin();
  Serial.println("DHT sensor initialized");
  
  // Connect to WiFi
  wifiManager.connect(WIFI_SSID, WIFI_PASSWORD);

  // Configure MQTT
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(callback);
  
  Serial.println("Setup complete!");
}

void loop() {
  // Maintain MQTT connection
  if (!mqtt_client.connected()) {
    reconnect_mqtt();
  }
  mqtt_client.loop();
  
  // Publish sensor data at interval
  unsigned long currentMillis = millis();
  if (currentMillis - lastPublish >= publishInterval) {
    lastPublish = currentMillis;
    publish_sensor_data();
  }
}
