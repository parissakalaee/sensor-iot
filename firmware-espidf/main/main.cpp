#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "credentials.h"

#include "wifi_manager.h"
#include "mqtt_wrapper.h"    
#include "sensor_reader.h"

static const char *TAG = "MAIN";

// Event group for WiFi connection status
static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

// Task handles
static TaskHandle_t sensor_task_handle = NULL;
static TaskHandle_t mqtt_task_handle = NULL;

// Forward declarations
void sensor_task(void *pvParameters);
void mqtt_task(void *pvParameters);

extern "C" void app_main(void)  
{
    ESP_LOGI(TAG, "=== ESP32 IoT Sensor (ESP-IDF) ===");
    
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Create event group
    wifi_event_group = xEventGroupCreate();
    
    // Initialize WiFi
    wifi_manager_init(WIFI_SSID, WIFI_PASSWORD, wifi_event_group, WIFI_CONNECTED_BIT);
    
    // Create tasks
    xTaskCreate(mqtt_task, "mqtt_task", 4096, NULL, 5, &mqtt_task_handle);
    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, &sensor_task_handle);
    
    ESP_LOGI(TAG, "Setup complete! Tasks waiting for WiFi...");
}

void mqtt_task(void *pvParameters)
{
    ESP_LOGI(TAG, "MQTT task started, waiting for WiFi...");
    
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, 
                       pdFALSE, pdTRUE, portMAX_DELAY);
    
    ESP_LOGI(TAG, "WiFi connected! Starting MQTT client...");
    
    mqtt_client_init(MQTT_BROKER, MQTT_TOPIC);
    
    vTaskDelete(NULL);
}

void sensor_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Sensor task started, waiting for WiFi...");
    
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT,
                       pdFALSE, pdTRUE, portMAX_DELAY);
    
    ESP_LOGI(TAG, "WiFi connected! Initializing sensor...");
    
    sensor_reader_init(GPIO_NUM_4);
    
    while (1) {
        SensorData data = sensor_reader_read();
        
        if (data.isValid) {
            ESP_LOGI(TAG, "Temp: %.1f°C, Humidity: %.1f%%", 
                    data.temperature, data.humidity);
            
            mqtt_client_publish_sensor_data(data);
        } else {
            ESP_LOGW(TAG, "Failed to read sensor");
        }
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}