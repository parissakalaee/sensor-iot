// firmware-espidf/main/wifi_manager.cpp
#include "wifi_manager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/event_groups.h"
#include <string.h>

static const char *TAG = "WiFiManager";

// Static variables for event handling
static EventGroupHandle_t s_wifi_event_group = NULL;
static int s_connected_bit = 0;
static int s_retry_count = 0;
static const int MAX_RETRY = 5;

/**
 * WiFi Event Handler - The heart of event-driven WiFi
 * 
 * This replaces your Arduino's blocking while loop!
 * Gets called automatically by ESP-IDF when WiFi events occur.
 */
static void wifi_event_handler(void* arg, 
                               esp_event_base_t event_base,
                               int32_t event_id, 
                               void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // WiFi started, begin connection attempt
        ESP_LOGI(TAG, "WiFi started, connecting...");
        esp_wifi_connect();
        
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // Connection lost or failed
        if (s_retry_count < MAX_RETRY) {
            esp_wifi_connect();
            s_retry_count++;
            ESP_LOGI(TAG, "Retry connection (%d/%d)", s_retry_count, MAX_RETRY);
        } else {
            ESP_LOGE(TAG, "Failed to connect after %d attempts", MAX_RETRY);
        }
        
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // Successfully connected and got IP!
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected! IP: " IPSTR, IP2STR(&event->ip_info.ip));
        
        s_retry_count = 0;
        
        // Signal other tasks that WiFi is ready
        if (s_wifi_event_group != NULL) {
            xEventGroupSetBits(s_wifi_event_group, s_connected_bit);
        }
    }
}

void wifi_manager_init(const char* ssid, 
                      const char* password,
                      EventGroupHandle_t event_group,
                      int connected_bit)
{
    ESP_LOGI(TAG, "Initializing WiFi...");
    
    // Store event group for signaling
    s_wifi_event_group = event_group;
    s_connected_bit = connected_bit;
    
    // Initialize network interface (required for WiFi)
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Create default event loop (handles WiFi events)
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Create default WiFi station interface
    esp_netif_create_default_wifi_sta();
    
    // Initialize WiFi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Register event handlers
    // These get called automatically when events occur!
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, 
                                               ESP_EVENT_ANY_ID, 
                                               &wifi_event_handler, 
                                               NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, 
                                               IP_EVENT_STA_GOT_IP, 
                                               &wifi_event_handler, 
                                               NULL));
    
    // Configure WiFi credentials
    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    
    // Set WiFi mode and configuration
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    // Start WiFi (non-blocking!)
    // Event handler will be called when connection succeeds
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG, "WiFi init complete. Connecting to SSID: %s", ssid);
}

bool wifi_manager_is_connected(void)
{
    if (s_wifi_event_group == NULL) {
        return false;
    }
    
    EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);
    return (bits & s_connected_bit) != 0;
}