#include <Arduino.h>
#include <Wifi.h>

char ssid[] = "neapy";
char password[] = "neapolitan";
int ledPin = 2;

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Still connecting...");
  }

  Serial.println("Connected!");
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}