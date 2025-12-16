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

#### Using Command Line:
```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Open serial monitor
pio device monitor

# Build + Upload + Monitor (all in one)
pio run --target upload && pio device monitor
```

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

## Troubleshooting

### Upload fails
- Check USB cable connection
- Verify correct COM port in Device Manager (Windows) or `ls /dev/cu.*` (Mac)
- Hold BOOT button on ESP32 while uploading

### WiFi not connecting
- Verify SSID and password
- Check 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Ensure router allows new devices

### Sensor reads NaN
- Check wiring (especially power and ground)
- Verify GPIO4 connection
- Try different GPIO pin and update `DHTPIN` in code

### MQTT not connecting
- Check internet connection
- Verify test.mosquitto.org is accessible
- Try different public broker (e.g., broker.hivemq.com)

## Next Steps
- [ ] Add OTA (Over-The-Air) updates
- [ ] Implement deep sleep for power saving
- [ ] Add watchdog timer for reliability
- [ ] Move to local MQTT broker
- [ ] Add TLS/SSL encryption
