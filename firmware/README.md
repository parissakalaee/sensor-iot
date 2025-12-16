# ESP32 Sensor Firmware

MQTT-enabled sensor firmware for ESP32 with DHT11 temperature and humidity sensor.

## Hardware Requirements
- ESP32 development board
- DHT11 sensor
- Connecting wires

## Wiring
```
DHT11 Pin 1 (VCC)  -> ESP32 3.3V
DHT11 Pin 2 (Data) -> ESP32 GPIO4 (D4)
DHT11 Pin 3 (NC)   -> Not connected
DHT11 Pin 4 (GND)  -> ESP32 GND
```

## Software Setup

### Prerequisites
1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)

### Configuration
1. Open `src/main.cpp`
2. Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

### Build & Upload

#### Using VS Code GUI:
1. Open this folder in VS Code
2. PlatformIO toolbar appears at bottom
3. Click "Build" (checkmark icon)
4. Click "Upload" (arrow icon)
5. Click "Serial Monitor" (plug icon)

## MQTT Configuration
- **Broker:** test.mosquitto.org:1883
- **Topic:** parissa/sensors
- **Payload Format:** `{"temperature": 23.5, "humidity": 65.0}`
- **Publish Interval:** 5 seconds

## Serial Output Example
```
=== ESP32 IoT Sensor ===
DHT sensor initialized
Connecting to WiFi: YourNetwork
.....
WiFi connected!
IP address: 192.168.1.100
Connecting to MQTT broker...Connected!
Published: {"temperature":23.60,"humidity":49.00}
Message received on topic: parissa/sensors. Message: {"temperature":23.60,"humidity":49.00}
```

