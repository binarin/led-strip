#include <esp_log.h>

#include "mqtt.hpp"

namespace MQTT {
  void init(const char* host, int port, const char* user, const char* password) {
  }
  void start() {
    ESP_LOGI("mqtt", "Starting mqtt connection");
  }
  void stop() {
    ESP_LOGI("mqtt", "Stopping mqtt connection");
  }
}
