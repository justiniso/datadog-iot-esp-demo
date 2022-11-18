# Datadog Metrics from ESP IoT devices

This is a demo library for submitting metrics to Datadog from ESP IoT devices (ESP32 / ESP8266 / ESP12).


## Usage


Usage is simple. See the examples director for an example using the client:

```cpp
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <DatadogClient.h>

#define DD_API_KEY "REPLACEME"
#define WIFI_SSID "REPLACEME"
#define WIFI_PASS "REPLACEME"

DatadogClient dd(DD_API_KEY);

void setup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  dd.begin();
}

void loop()
{
  dd.metric("demo.esp.test", METRIC_TYPE_GAUGE, 100, "\"tag1:demo\",\"tag2:demo2\"");
  delay(10000);
}
```
