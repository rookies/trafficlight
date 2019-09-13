#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Config.hh"

byte packetSize = 14;
WiFiUDP server;

void setup()
{
  /* Setup serial port: */
  Serial.begin(115200);
  Serial.println();

  /* Setup wifi connection: */
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, passphrase);

  /* Connect to wifi: */
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected.");

  /* Start server: */
  server.begin(port);
  Serial.print("Server started at UDP ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);

  /* Setup OTA updates: */
  ArduinoOTA.setPort(otaPort);
  ArduinoOTA.setHostname(otaHostname);
  ArduinoOTA.setPassword(otaPassword);
  ArduinoOTA.begin();

  /* Set pins: */
  pinMode(pinRed, OUTPUT);
  pinMode(pinYellow, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  analogWrite(pinRed, 1023);
  analogWrite(pinYellow, 1023);
  analogWrite(pinGreen, 1023);
}

void loop() {
  /* Handle OTA updates: */
  ArduinoOTA.handle();

  /* Handle UDP packets: */
  char buf[packetSize+1];
  unsigned int v1,v2,v3;
  if (server.parsePacket()) {
    /* Read message into the buffer: */
    int num = server.read(buf, packetSize);
    buf[num] = '\0';
    /* Print it: */
    Serial.print("Received: ");
    Serial.println(buf);
    /* Split & parse it: */
    /* Message format: v1,v2,v3 with each v = 0..1023 */
    char *comma1 = strchr(buf, ',');
    if (comma1) {
      *comma1 = '\0';
      v1 = atoi(buf);
      char *comma2 = strchr(comma1 + 1, ',');
      if (comma2) {
        *comma2 = '\0';
        v2 = atoi(comma1 + 1);
        v3 = atoi(comma2 + 1);
        /* Print debug output: */
        Serial.print("Setting outputs to: (");
        Serial.print(v1);
        Serial.print(",");
        Serial.print(v2);
        Serial.print(",");
        Serial.print(v3);
        Serial.println(")");
        /* Set outputs: */
        analogWrite(pinRed, v1);
        analogWrite(pinYellow, v2);
        analogWrite(pinGreen, v3);
      }
    }
  }
}
