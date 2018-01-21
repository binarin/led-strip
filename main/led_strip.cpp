#include <esp_log.h>
#include "led_strip.hpp"

namespace {
  const int LED_STRIP_LENGTH = 2;
  led_color_t buf_1[LED_STRIP_LENGTH];
  led_color_t buf_2[LED_STRIP_LENGTH];

  led_strip_t strip = {
    .rgb_led_type = RGB_LED_TYPE_WS2812,
    .led_strip_length = 2,
    .rmt_channel = RMT_CHANNEL_0,
    .rmt_interrupt_num = 20,
    .gpio = GPIO_NUM_15,
    .showing_buf_1 = true,
    .led_strip_buf_1 = buf_1,
    .led_strip_buf_2 = buf_2,
  };

  bool state;
}

namespace LedStrip {
  void start() {
    strip.access_semaphore = xSemaphoreCreateBinary();

    bool led_init_ok = led_strip_init(&strip);
    if (!led_init_ok) {
      ESP_LOGE("blink", "led strip init failed!");
      return;
    }
    ESP_LOGI("blink", "led strip Init complete...");
    led_strip_clear(&strip);
    led_strip_show(&strip);
    state = false;
  }
  bool isOn() {
    return state;
  }
  void turnOn() {
    state = true;

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
  void turnOff() {
    state = false;
    led_strip_clear(&strip);
    led_strip_show(&strip);
  }
}
