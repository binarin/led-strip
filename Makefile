#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := blink

include $(IDF_PATH)/make/project.mk

CXXFLAGS += -DENV_SSID=\"agares\" -DENV_WIFI_PASSWORD=\"${WIFI_PASSWORD}\" -DENV_MQTT_BROKER=\"192.168.2.11\" -DENV_MQTT_USER=\"led-strip-1\" -DENV_MQTT_PASSWORD=\"${MQTT_PASSWORD}\"
