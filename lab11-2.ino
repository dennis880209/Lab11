#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"

#define _SSID "Yuan"
#define _KEY "12345678"

MCSDevice mcs("DZwYqIhM", "x3SX3ZK7FkZDgDxB");

MCSControllerOnOff led("123");
MCSDisplayOnOff remote("321");
MCSDisplayInteger Temp("11");

#define LED_PIN 7

void setup()
{
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(A0, OUTPUT);
  while (WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  mcs.addChannel(led);
  mcs.addChannel(remote);
  mcs.addChannel(Temp);
  while (!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  while (!led.valid())
  {
    Serial.println("read LED value from MCS...");
    led.value();
  }
  Serial.print("done, LED value = ");
  Serial.println(led.value());
  digitalWrite(LED_PIN, led.value() ? HIGH : LOW);
}

void loop() 
{
  Serial.print("process(");
  Serial.print(millis());
  Serial.println(")");
  mcs.process(100);
  if (led.updated())
  {
    Serial.print("LED updated, new value = ");
    Serial.println(led.value());
    digitalWrite(LED_PIN, led.value() ? HIGH : LOW);
    if (!remote.set(led.value()))
    {
      Serial.print("Failed to update remote");
      Serial.println(remote.value());
    }
  }
  while (!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if (mcs.connected())
      Serial.println("MCS connected !!");
  }
  Temp.set(analogRead(A0));
}
