#include "mqtt_wrapper.h"
#include <mqtt_client.h> 
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "MqttClient";

// MQTT client handle
static esp_mqtt_client_handle_t mqtt_client = NULL;
static const char* publish_topic = NULL;
static bool is_connected = false;

// External reference to embedded certificate
// This will be embedded by CMakeLists.txt EMBED_FILES
extern const uint8_t ca_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t ca_cert_pem_end[]   asm("_binary_ca_cert_pem_end");

/**
 * MQTT Event Handler - handles connection, disconnection, data
 * 
 * This is the event-driven heart of MQTT (like WiFi event handler)
 */
static void mqtt_event_handler(void *handler_args, 
                               esp_event_base_t base, 
                               int32_t event_id, 
                               void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected!");
            is_connected = true;
            
            // Subscribe to topic (optional - for receiving messages)
            esp_mqtt_client_subscribe(mqtt_client, publish_topic, 0);
            ESP_LOGI(TAG, "Subscribed to topic: %s", publish_topic);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT Disconnected");
            is_connected = false;
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT Subscribed, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT Published, msg_id=%d", event->msg_id);
            break;
            
        case MQTT_EVENT_DATA:
            // Message received on subscribed topic
            ESP_LOGI(TAG, "MQTT Data received:");
            ESP_LOGI(TAG, "  Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "  Data: %.*s", event->data_len, event->data);
            break;
            
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT Error!");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGE(TAG, "  Last errno: %d", event->error_handle->esp_transport_sock_errno);
            }
            break;
            
        default:
            ESP_LOGD(TAG, "MQTT Event: %d", event->event_id);
            break;
    }
}

void mqtt_client_init(const char* broker_uri, const char* topic)
{
    ESP_LOGI(TAG, "Initializing MQTT client...");
    ESP_LOGI(TAG, "  Broker: %s", broker_uri);
    ESP_LOGI(TAG, "  Topic: %s", topic);
    
    publish_topic = topic;
    
    // MQTT client configuration with TLS
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = broker_uri;
    
    // TLS Configuration
    // mqtt_cfg.broker.verification.certificate = (const char*)ca_cert_pem_start;
    // mqtt_cfg.broker.verification.certificate_len = ca_cert_pem_end - ca_cert_pem_start;
    
    // Client identification
    mqtt_cfg.credentials.client_id = "ESP32-SensorIoT";
    
    // Connection settings
    mqtt_cfg.network.timeout_ms = 10000;
    mqtt_cfg.network.refresh_connection_after_ms = 60000;
    
    // Session settings
    mqtt_cfg.session.keepalive = 60;
    mqtt_cfg.session.disable_clean_session = false;
    
    // Create MQTT client
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client!");
        return;
    }
    
    // Register event handler
    esp_mqtt_client_register_event(mqtt_client, 
                                   (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, 
                                   mqtt_event_handler, 
                                   NULL);
    
    // Start MQTT client (non-blocking, connects in background)
    esp_mqtt_client_start(mqtt_client);
    
    ESP_LOGI(TAG, "MQTT client started");
}

bool mqtt_client_publish_sensor_data(const SensorData& data)
{
    if (!is_connected) {
        ESP_LOGW(TAG, "MQTT not connected, cannot publish");
        return false;
    }
    
    // Create JSON payload
    // Format: {"temperature":25.5,"humidity":60.2,"timestamp":12345}
    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"temperature\":%.1f,\"humidity\":%.1f,\"timestamp\":%lu}",
             data.temperature,
             data.humidity,
             (unsigned long)(esp_timer_get_time() / 1000)); // milliseconds
    
    // Publish message
    int msg_id = esp_mqtt_client_publish(mqtt_client, 
                                         publish_topic, 
                                         payload, 
                                         0,  // length (0 = auto-calculate)
                                         1,  // QoS 1 (at least once delivery)
                                         0); // not retained
    
    if (msg_id >= 0) {
        ESP_LOGI(TAG, "Published: %s", payload);
        return true;
    } else {
        ESP_LOGE(TAG, "Publish failed!");
        return false;
    }
}

bool mqtt_client_is_connected(void)
{
    return is_connected;
}