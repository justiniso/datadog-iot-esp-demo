
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <DatadogClient.h>

#define DD_API_KEY "REPLACEME"
#define WIFI_SSID "REPLACEME"
#define WIFI_PASS "REPLACEME"

#define GPIO_BUTTON D5

DatadogClient dd(DD_API_KEY);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;

void setup()
{
  Serial.begin(115200);

  pinMode(GPIO_BUTTON, INPUT_PULLUP);

  // Connect to WiFI
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Mac Addr: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  dd.begin();
}

void loop()
{
  int value = -100;
  if (digitalRead(GPIO_BUTTON) == LOW)
  {
    value = 100;
  }
  dd.metric("demo.esp.alerted", METRIC_TYPE_GAUGE, value, "\"purpose:demo\",\"location:hallway\"");
}