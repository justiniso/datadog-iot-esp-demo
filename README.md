# Monitor ESP IoT devices with Datadog (ESP8266 / ESP32)

![IMG_6175](https://user-images.githubusercontent.com/717862/202780328-f13bdd41-2c79-4e0c-ac54-f5ceabed8b2b.jpg)

This is a demo library for sending metrics to Datadog from ESP-based IoT devices (ESP32 / ESP8266 / ESP12). The [Datadog IoT Agent](https://docs.datadoghq.com/agent/iot/?tab=deb) cannot run on ESP-based microprocessors, but these ESP-based microprocessors are widely used in IoT devices. They usually require a centralized hub running MQTT to collect statistics, but the hub-based architecture presents some challenges in certain applications:

* A second device is needed
* It is more expensive to run a hub than cheap ESP chips
* Hubs are a single point of failure for all telemetry

By sending telemetry directly from the ESP microprocessors, the decentralized architecture is far better suited for certain applications (e.g. security).

## Usage

Usage in application code is simple. You can initialize the Datadog client once and submit metrics. Due to the high cost of establishing a new connection, it is generally recommended to pre-aggregate events before flushing them to Datadog.

See the examples director for an example using the client:

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
