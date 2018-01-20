#ifndef __WIFI_HPP__
#define __WIFI_HPP__

// #include <functional>
// #include <string>
// #include <esp_wifi.h>

// struct WiFi {
//   std::string ssid, password;
//   std::function<void()> connectCallback, disconnectCallback;

//   WiFi(std::string ssid, std::string password, std::function<void()> conn, std::function<void()> disconn);

//   static esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
// }

namespace WiFi {
  void init(const char *ssid, const char *password);
  void start();
}

#endif // __WIFI_HPP__
