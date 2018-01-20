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
#include "led_strip/led_strip.h"
#include <nvs_flash.h>

#include "wifi.hpp"
#include "mqtt.hpp"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO ((gpio_num_t)CONFIG_BLINK_GPIO)

void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_strip() {
  static struct led_color_t buf_1[2];
  static struct led_color_t buf_2[2];

  static struct led_strip_t strip = {
    .rgb_led_type = RGB_LED_TYPE_WS2812,
    .led_strip_length = 2,
    .rmt_channel = RMT_CHANNEL_0,
    .rmt_interrupt_num = 20,
    .gpio = GPIO_NUM_15,
    .showing_buf_1 = true,
    .led_strip_buf_1 = buf_1,
    .led_strip_buf_2 = buf_2,
  };
  strip.access_semaphore = xSemaphoreCreateBinary();

  bool led_init_ok = led_strip_init(&strip);
  if (!led_init_ok) {
    ESP_LOGE("blink", "led strip init failed!");
    return;
  }
  ESP_LOGI("blink", "led strip Init complete...");
  led_strip_clear(&strip);
  struct led_color_t red = {
    .red = 255,
    .green = 0,
    .blue = 0
  };
  led_strip_set_pixel_color(&strip, 0, &red);

  red.red = 0;
  red.green = 255;
  led_strip_set_pixel_color(&strip, 1, &red);

  led_strip_show(&strip);
}

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

  init_strip();
}
