#ifndef __MQTT_HPP__
#define __MQTT_HPP__

namespace MQTT {
  void init(const char* host, int port, const char* user, const char* password);
  void start();
  void stop();
}

#endif // __MQTT_HPP__
