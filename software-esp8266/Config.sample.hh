#pragma once
#include <ESP8266WiFi.h>

/* Pin configuration: */
const byte pinRed = D1;
const byte pinYellow = D2;
const byte pinGreen = D3;

/* Network configuration: */
IPAddress ip(192,168,1,100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
const char *ssid = "xxxxxxxx";
const char *passphrase = "xxxxxxxx";
unsigned int port = 1337;
