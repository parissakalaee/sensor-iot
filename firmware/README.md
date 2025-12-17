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
```bash
cd firmware

# Configure WiFi and MQTT credentials
cp src/credentials.h.example src/credentials.h
# Edit credentials.h with your settings
```

**credentials.h structure:**
```cpp
const char* WIFI_SSID = "your-wifi-ssid";
const char* WIFI_PASSWORD = "your-wifi-password";

// For HiveMQ Cloud (when USE_TLS enabled)
const char* HIVEMQ_BROKER = "your-cluster.hivemq.cloud";
const char* HIVEMQ_USER = "your-username";
const char* HIVEMQ_PASS = "your-password";
```

### Build & Upload

#### Using VS Code GUI:
1. Open this folder in VS Code
2. PlatformIO toolbar appears at bottom
3. Click "Build" (checkmark icon)
4. Click "Upload" (arrow icon)
5. Click "Serial Monitor" (plug icon)

## MQTT Configuration

### Broker Selection
Toggle between brokers in `src/config.h`:
```cpp
#define USE_HIVEMQ_CLOUD false  // Set to true for HiveMQ Cloud with TLS
```

**Plain MQTT (default):**
- **Broker:** test.mosquitto.org:1883
- **Authentication:** None required

**HiveMQ Cloud with TLS:**
- **Broker:** Configured in credentials.h
- **Port:** 8883
- **Authentication:** Username/password required

### Data Format
- **Topic:** parissa/sensors
- **Payload Format:** `{"temperature": 23.5, "humidity": 65.0, "timestamp": 12345}`
- **Publish Interval:** 5 seconds

## TLS Implementation Status

⚠️ **TLS over MQTT is currently not working due to ESP32 Arduino framework limitations.**

### What was attempted:
- HiveMQ Cloud integration with TLS encryption (port 8883)
- WiFiClientSecure with NTP time synchronization
- Certificate validation bypass using `setInsecure()`

### Current status:
- ✅ Plain MQTT works reliably (test.mosquitto.org)
- ✅ HiveMQ credentials verified (tested with MQTT Explorer)
- ✅ TLS handshake completes successfully
- ❌ Connection fails when PubSubClient attempts MQTT over TLS
- Error: `-29312 SSL - The connection indicated an EOF`

### Root cause:
Known compatibility issue between WiFiClientSecure and PubSubClient library on ESP32 Arduino framework. Production deployments should use ESP-IDF framework instead.

### Architecture benefits:
The code structure supports both plain and TLS modes via compile-time flags, making it easy to switch between brokers for development vs. production.

## Serial Output Example
```
=== ESP32 IoT Sensor ===
DHT sensor initialized
Connecting to WiFi...
WiFi connected!
IP address: 192.168.1.100
Connecting to MQTT broker...Connected!
Published: {"temperature":23.60,"humidity":49.00,"timestamp":5127}
```