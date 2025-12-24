#include "sensor_reader.h"
#include "dht.h"
#include "esp_log.h"

static const char *TAG = "SensorReader";
static gpio_num_t dht_gpio = GPIO_NUM_NC;

void sensor_reader_init(gpio_num_t gpio_pin)
{
    dht_gpio = gpio_pin;
    ESP_LOGI(TAG, "DHT11 initialized on GPIO %d", gpio_pin);
}

SensorData sensor_reader_read(void)
{
    SensorData result = {0.0f, 0.0f, false};
    
    if (dht_gpio == GPIO_NUM_NC) {
        ESP_LOGE(TAG, "Sensor not initialized!");
        return result;
    }
    
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    // Read from DHT11
    esp_err_t res = dht_read_data(DHT_TYPE_DHT11, dht_gpio, &humidity, &temperature);
    
    if (res == ESP_OK) {
        // DHT library returns values * 10, so divide by 10
        result.temperature = temperature / 10.0f;
        result.humidity = humidity / 10.0f;
        result.isValid = true;
        
        ESP_LOGD(TAG, "Read successful: %.1f°C, %.1f%%", 
                result.temperature, result.humidity);
    } else {
        ESP_LOGW(TAG, "Failed to read DHT11: %s", esp_err_to_name(res));
        result.isValid = false;
    }
    
    return result;
}