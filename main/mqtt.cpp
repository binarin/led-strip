#include <esp_log.h>
#include <mqtt.h>
#include <string.h>

#include "mqtt.hpp"
#include "led_strip.hpp"

namespace {
  const char* STATE_TOPIC = "home/office/" ENV_DEVICE_ID;
  const char* COMMAND_TOPIC = "home/office/" ENV_DEVICE_ID "/set";
  const char* AVAILABILITY_TOPIC = "home/office/" ENV_DEVICE_ID "/available";
  const char* PAYLOAD_AVAILABLE = "ONLINE";
  const char* PAYLOAD_NOT_AVAILABLE = "OFFLINE";
  const char* PAYLOAD_ON = "ON";
  const char* PAYLOAD_OFF = "OFF";

  void publish_led_state(mqtt_client* client) {
    const char* statePayload = LedStrip::isOn() ? PAYLOAD_ON : PAYLOAD_OFF;
    mqtt_publish(client, STATE_TOPIC, statePayload, strlen(statePayload), 0, 1);
  }


  void connected_cb(mqtt_client* client, mqtt_event_data_t *event_data) {
    mqtt_subscribe(client, COMMAND_TOPIC, 0);
    mqtt_publish(client, AVAILABILITY_TOPIC, PAYLOAD_AVAILABLE, strlen(PAYLOAD_AVAILABLE), 0, 0);
    publish_led_state(client);
  }

  void disconnected_cb(mqtt_client* client, mqtt_event_data_t *event_data) {
  }

  void subscribe_cb(mqtt_client* client, mqtt_event_data_t *event_data) {
    ESP_LOGI("mqtt", "Subscribed to %s", COMMAND_TOPIC);
  }

  void publish_cb(mqtt_client* client, mqtt_event_data_t *event_data) {
  }

  void data_cb(mqtt_client *client, mqtt_event_data_t *event_data) {
    if(event_data->data_offset == 0) {
      char *topic = (char*)malloc(event_data->topic_length + 1);
      memcpy(topic, event_data->topic, event_data->topic_length);
      topic[event_data->topic_length] = 0;
      ESP_LOGI("mqtt", "[APP] Publish topic: %s", topic);
      free(topic);
    }

    char *data = (char*)malloc(event_data->data_length + 1);
    memcpy(data, event_data->data, event_data->data_length);
    data[event_data->data_length] = 0;
    if (!strcmp(data, PAYLOAD_ON)) {
      LedStrip::turnOn();
    } else if (!strcmp(data, PAYLOAD_OFF)) {
      LedStrip::turnOff();
    }
    publish_led_state(client);

    ESP_LOGI("mqtt", "[APP] Publish data[%d/%d bytes]: %s",
             event_data->data_length + event_data->data_offset,
             event_data->data_total_length,
             data);

    free(data);
  }
}

namespace MQTT {
  mqtt_settings *settings;

  void init(const char* host, int port, const char* user, const char* password) {
    settings = new mqtt_settings();

    settings->connect_cb = nullptr;
    settings->disconnect_cb = nullptr;
    settings->read_cb = nullptr;
    settings->write_cb = nullptr;
    settings->connected_cb = connected_cb;
    settings->disconnected_cb = disconnected_cb;
    settings->subscribe_cb = subscribe_cb;
    settings->publish_cb = publish_cb;
    settings->data_cb = data_cb;

    strlcpy(settings->host, host, sizeof(settings->host));
    settings->port = port;

    strlcpy(settings->client_id, "led-strip-1", sizeof(settings->client_id));
    strlcpy(settings->username, user, sizeof(settings->username));
    strlcpy(settings->password, password, sizeof(settings->password));

    strlcpy(settings->lwt_topic, AVAILABILITY_TOPIC, sizeof(settings->lwt_topic));
    settings->lwt_msg_len = strlcpy(settings->lwt_msg, PAYLOAD_NOT_AVAILABLE, sizeof(settings->lwt_msg));
    settings->lwt_qos = 0;
    settings->lwt_retain = 1;
    settings->clean_session = 0;
    settings->keepalive = 60;
    settings->auto_reconnect = true;
  }

  void start() {
    ESP_LOGI("mqtt", "Starting mqtt connection");
    mqtt_start(settings);
  }

  void stop() {
    ESP_LOGI("mqtt", "Stopping mqtt connection");
    mqtt_stop();
  }
}
