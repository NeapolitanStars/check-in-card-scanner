#include <Arduino.h>
#include <Wifi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <HTTPClient.h>

// SSID and password for the ESP-32 to connect to WiFi
char ssid[] = "neapy";
char password[] = "neapolitan"; // Yes you may have my hotspot password

// Initialize pins
int led_pin = 2;
int ss_pin = 19;  // For card reader
int rst_pin = 15; // For card reader

// Initialize LCD screen
LiquidCrystal lcd(26, 25, 33, 32, 35, 34); // RS, E, D4, D5, D6, D7

// Initialize mfrc522, which allows reading/writing to RFID tags
MFRC522 rfid(ss_pin, rst_pin);

void setup()
{
  pinMode(led_pin, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);
  SPI.begin(); // Initialize Serial Peripheral Interface (SPI), which allows the ESP-32 to communicate with the NFID reader
  rfid.PCD_Init(); // Initialize the RFID reader

  WiFi.begin(ssid, password); // Connects the ESP-32 to Wi-Fi
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
  // Wait for a card to be present on the reader
  if(!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
    return;
  }

  // Read the card's UID
  String cardID = "";
  for (byte i = 0; i < rfid.uid.size; i++){
    cardID += String(rfid.uid.uidByte[i], HEX);
  }
  cardID.toUpperCase();

  Serial.println("Card ID: " + cardID);

  
  // Send card ID to Google Apps Script
  HTTPClient http;
}