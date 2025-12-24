#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize WiFi in station mode and start connection
 * 
 * @param ssid WiFi network name
 * @param password WiFi password
 * @param event_group Event group to signal connection status
 * @param connected_bit Bit to set when connected
 */
void wifi_manager_init(const char* ssid, 
                      const char* password,
                      EventGroupHandle_t event_group,
                      int connected_bit);

/**
 * Check if WiFi is connected
 * @return true if connected, false otherwise
 */
bool wifi_manager_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_H