#include <DatadogClient.h>

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#endif
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

DatadogClient::DatadogClient(const char *apiKey)
{
  _apiKey = apiKey;
}

DatadogClient::DatadogClient(const char *apiKey, const char *appKey)
{
  _apiKey = apiKey;
  _appKey = appKey; // TODO: unused
}

void DatadogClient::begin()
{
  timeClient.begin();
  timeClient.update();
}

void DatadogClient::metric(char *name, uint8_t type, int value)
{
  return metric(name, type, value, "");
}

void DatadogClient::metric(char *name, uint8_t type, int value, char *tags)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to send metrics due to WiFi not connected");
    return;
  }

  // Update NTP timestamp
  timeClient.update();

  char payload[1000];
  sprintf(payload, "{\"series\":[{\"metric\":\"%s\",\"type\":%d,\"points\":[{\"timestamp\":%d,\"value\":%d}],\"tags\":[%s]}]}", name, type, timeClient.getEpochTime(), value, tags);
  Serial.print("Sending payload: ");
  Serial.println(payload);

#ifdef ESP32
  WiFiClientSecure client;
  client.setInsecure(); // TODO: Do not use insecure requests; use the cert
  client.stop();
  if (!client.connect("https://api.datadoghq.com", 443))
  {
    Serial.println(F("Connection wasnt established"));
    return;
  }
  char apiKeyHeader[100];
  sprintf(apiKeyHeader, "DD-API-KEY: %s", _apiKey);
  client.println("POST https://api.datadoghq.com/api/v2/series HTTP/1.0");
  client.println("Host: api.datadoghq.com");
  client.println(F("Connection: close"));
  client.println(F("Content-Type: application/json;"));
  client.println(apiKeyHeader);
  client.print(F("Content-Length: "));
  client.println(String(payload).length());
  client.println();
  client.println(payload);
  while (client.connected())
  {
    String line = client.readStringUntil('\n'); // HTTP headers
    if (line == "\r")
    {
      break;
    }
  }
  Serial.println("Received response:\n=====================================");
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
  Serial.println("\n=====================================");

  client.stop();

#else
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure(); // TODO: Do not use insecure requests; use the cert

  HTTPClient https;
  https.begin(*client, "https://api.datadoghq.com/api/v2/series");
  https.addHeader("Accept", "application/json");
  https.addHeader("Content-Type", "application/json");
  https.addHeader("DD-API-KEY", _apiKey);

  int responseCode = https.POST(payload);
  Serial.print("Received datadog response: ");
  Serial.println(responseCode);

  if (responseCode != 202)
  {
    const String &response = https.getString();
    Serial.println("Received response:\n=====================================");
    Serial.println(response);
    Serial.println(https.header("DD-API-KEY"));
    Serial.println("\n=====================================");
  }
#endif
}