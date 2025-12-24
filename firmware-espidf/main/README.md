# ESP32 AWS IoT Core Firmware

ESP32 firmware connecting DHT11 sensor to AWS IoT Core with secure MQTT over TLS.

## Architecture

```
DHT11 Sensor → ESP32 (ESP-IDF) → AWS IoT Core (MQTT/TLS) → Cloud
```

## Features

- **Secure Communication**: X.509 certificate authentication with AWS IoT Core
- **Real-time Publishing**: Temperature & humidity readings every 5 seconds
- **Production Framework**: Built with ESP-IDF (Espressif's official framework)
- **Automatic Reconnection**: WiFi and MQTT connection management

## Hardware

- ESP32 DevKit
- DHT11 temperature & humidity sensor (GPIO 4)

## Tech Stack

- **Framework**: ESP-IDF
- **Protocol**: MQTT over TLS (port 8883)
- **Cloud**: AWS IoT Core
- **Language**: C/C++

## Configuration

1. Copy `main/credentials.h.example` to `main/credentials.h`
2. Add your WiFi credentials
3. Add your AWS IoT endpoint
4. Place AWS IoT certificates in `main/certs/`:
   - `aws_root_ca.pem`
   - `device_cert.pem`
   - `device_key.pem`

## Build & Flash

```bash
idf.py build
idf.py -p /dev/cu.usbserial-XXXX flash monitor
```

## MQTT Topic

Publishes to: `esp32/sensors`

Payload format:
```json
{
  "temperature": 25.5,
  "humidity": 60.2,
  "timestamp": 1234567890
}
```

## Project Context

Part of learning path combining embedded systems with cloud IoT platforms. Demonstrates:
- Production ESP32 firmware development
- AWS IoT device provisioning
- Secure IoT communication patterns

---

**Note**: This is a learning project exploring professional IoT development practices.