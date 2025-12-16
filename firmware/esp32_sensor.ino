#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Configuration
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* topic = "parissa/sensors";

// DHT11 Sensor Configuration
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastPublish = 0;
const unsigned long PUBLISH_INTERVAL = 5000; // 5 seconds

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT broker...");
        String clientId = "ESP32-" + String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("Connected!");
            client.subscribe(topic);
            Serial.print("Subscribed to topic: ");
            Serial.println(topic);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void connectWiFi() {
    Serial.println("\n=== ESP32 IoT Sensor ===");
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    randomSeed(micros());

    // Initialize DHT sensor
    dht.begin();
    Serial.println("DHT11 sensor initialized");

    // Connect to WiFi
    connectWiFi();

    // Setup MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    // Ensure MQTT connection
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    // Publish sensor data at regular intervals
    if (millis() - lastPublish >= PUBLISH_INTERVAL) {
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (!isnan(humidity) && !isnan(temperature)) {
            // Create JSON message
            String message = "{\"temperature\":" + String(temperature, 1) +
                             ",\"humidity\":" + String(humidity, 1) + "}";

            if (client.publish(topic, message.c_str())) {
                Serial.print("Published: ");
                Serial.println(message);
            } else {
                Serial.println("Publish failed!");
            }
        } else {
            Serial.println("Failed to read from DHT sensor!");
        }

        lastPublish = millis();
    }
}