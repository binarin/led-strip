/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <cstdio>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/rmt.h>
#include "sdkconfig.h"
#include <nvs_flash.h>

#include "wifi.hpp"
#include "mqtt.hpp"
#include "led_strip.hpp"

extern "C" void app_main()
{
  ESP_LOGI("MQTT", "[APP] Startup..");
  ESP_LOGI("MQTT", "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI("MQTT", "[APP] SDK version: %s", esp_get_idf_version());

  // printf("EXT CFG: %s\n", STRING(ENV_SSID));
  // xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

  nvs_flash_init();
  WiFi::init(ENV_SSID, ENV_WIFI_PASSWORD);
  MQTT::init(ENV_MQTT_BROKER, 1883, ENV_MQTT_USER, ENV_MQTT_PASSWORD);

  WiFi::start();
  LedStrip::start();
}
