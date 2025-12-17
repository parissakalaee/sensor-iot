#include "wifi_manager.h"

void WiFiManager::connect(const char* ssid, const char* password) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}