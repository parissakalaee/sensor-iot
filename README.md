# Sensor IoT Dashboard

Android app for monitoring IoT sensors via MQTT, built with Jetpack Compose and modern Android architecture.

## Current Status
- ✅ Jetpack Compose UI with Material 3
- ✅ MVVM architecture with StateFlow
- ✅ Hilt dependency injection
- ✅ MQTT integration (Eclipse Paho)
- ✅ Real-time sensor updates from ESP32
- ✅ Thread-safe coroutines with structured concurrency

## Features
- Real-time temperature and humidity monitoring from DHT11 sensor
- MQTT communication via public broker (test.mosquitto.org)
- Connection status indicators
- Manual sensor connect/disconnect controls
- Timestamp tracking for last update

## Tech Stack
**Android:**
- **Language:** Kotlin
- **UI:** Jetpack Compose, Material 3
- **Architecture:** MVVM with clean separation (ViewModel → Repository → Data Source)
- **DI:** Hilt (Dagger)
- **Async:** Coroutines, Flow, StateFlow
- **IoT:** MQTT (Eclipse Paho)

**Hardware:**
- **Board:** ESP32
- **Sensor:** DHT11 (temperature & humidity)
- **Framework:** Arduino
- **Libraries:** WiFi, PubSubClient, DHT

## Hardware Setup
- ESP32 development board
- DHT11 sensor connected to GPIO 4
- MQTT broker: test.mosquitto.org:1883
- Topic: `parissa/sensors`
- Message format: `{"temperature": 23.5, "humidity": 65.0}`
- Update interval: 5 seconds

## Architecture
```
UI (Compose) 
    ↓ observes StateFlow
ViewModel (Hilt injected)
    ↓ collects Flow
SensorRepository 
    ↓ delegates to
MqttRepository (callbackFlow)
    ↓ subscribes via MQTT
test.mosquitto.org 
    ↑ publishes from
ESP32 + DHT11 Sensor
```

## Running the Project

**1. ESP32 Setup:**
```bash
# Open firmware/esp32_sensor.ino in Arduino IDE
# Install required libraries:
# - PubSubClient (by Nick O'Leary)
# - DHT sensor library (by Adafruit)
# Update WiFi credentials in the code
# Upload to ESP32
```

## Project Structure
```
sensor-iot/
├── app/                    # Android application
│   ├── data/              # Repository & data models
│   ├── di/                # Hilt dependency injection
│   ├── ui/                # Compose UI components
│   └── viewmodels/        # ViewModels
└── firmware/              # ESP32 firmware
    └── esp32_sensor      # Arduino sketch
```

## Learning Journey
This project demonstrates:
1. ✅ Modern Android development (Compose, Hilt, Coroutines)
2. ✅ Reactive programming with Flow
3. ✅ Thread-safe state management with StateFlow.update{}
4. ✅ Converting callbacks to Flows with callbackFlow
5. ✅ Hardware integration (ESP32 + DHT11)
6. ✅ MQTT protocol for IoT communication