#include <tcpip_adapter.h>
#include <freertos/event_groups.h>
#include <esp_event_loop.h>
#include <esp_wifi.h>
#include <cstring>

#include "mqtt.hpp"

namespace {
  EventGroupHandle_t wifi_event_group;
  const int CONNECTED_BIT = BIT0;

  esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
  {
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
      esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
      MQTT::start();
      //init app here
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      /* This is a workaround as ESP32 WiFi libs don't currently
         auto-reassociate. */
      esp_wifi_connect();
      MQTT::stop();
      xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
      break;
    default:
      break;
    }
    return ESP_OK;
  }
}

namespace WiFi {
  void init(const char* ssid, const char* password)
  {
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    wifi_config_t wifi_config;
    std::strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = '\0';
    std::strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));
    wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = '\0';

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  }

  void start() {
    ESP_ERROR_CHECK(esp_wifi_start());
  }
}
