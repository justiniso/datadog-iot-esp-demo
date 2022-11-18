#ifndef _DATADOG_CLIENT_h_
#define _DATADOG_CLIENT_h_

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#endif

#define METRIC_TYPE_COUNT 1
#define METRIC_TYPE_RATE 2
#define METRIC_TYPE_GAUGE 3

class DatadogClient
{
public:
    DatadogClient(const char *apiKey);
    DatadogClient(const char *apiKey, const char *appKey);
    void begin();
    void metric(char *name, uint8_t type, int value);
    void metric(char *name, uint8_t type, int value, char *tags);

private:
    const char *_apiKey;
    const char *_appKey;
};

#endif