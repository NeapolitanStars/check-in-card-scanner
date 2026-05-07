#include <Arduino.h>
#include <Wifi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>

// SSID and password for the ESP-32 to connect to WiFi
char ssid[] = "neapy";
char password[] = "neapolitan";

// Initialize pins
int led_pin = 2;
int ss_pin = 4;  // dummy value
int rst_pin = 3; // dummy value

// Initialize LCD screen
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // dummy value

// Initialize mfrc522, which allows reading/writing to RFID tags
MFRC522 mfrc522(ss_pin, rst_pin);

void setup()
{
  pinMode(led_pin, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);
  SPI.begin(); // Initialize Serial Peripheral Interface (SPI), which allows the ESP-32 to communicate with the NFID reader

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Still connecting...");
  }

  digitalWrite(led_pin, HIGH);
  Serial.println("Connected!");
}

void loop()
{
}