#include <Arduino.h>
#include <WiFi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <HTTPClient.h>

// SSID and password for the ESP-32 to connect to WiFi
char ssid[] = "neapy";
char password[] = "neapolitan"; // Yes you may have my hotspot password

// Initialize pins
int led_pin = 2;
int ss_pin = 5;  // For card reader
int rst_pin = 4; // For card reader

// Initialize LCD screen
LiquidCrystal lcd(14, 27, 26, 25, 33, 32); // RS, E, D4, D5, D6, D7

// Initialize mfrc522, which allows reading/writing to RFID tags
MFRC522 rfid(ss_pin, rst_pin);

void setup()
{
  pinMode(led_pin, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);
  SPI.begin();     // Initialize Serial Peripheral Interface (SPI), which allows the ESP-32 to communicate with the RFID reader
  rfid.PCD_Init(); // Initialize the RFID reader

  // Connects the ESP-32 to Wi-Fi and prints connection status to Serial Monitor and LCD screen
  WiFi.begin(ssid, password); 
  Serial.println("Connecting to WiFi...");
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");

  // Wait until the ESP-32 is connected to Wi-Fi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Still connecting...");
  }

  // Once connected, turn on the built-in LED and print to Serial Monitor and LCD screen
  digitalWrite(led_pin, HIGH);
  Serial.println("Connected!");
  lcd.clear();
  lcd.print("Connected to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi!");
  delay(2000);
  lcd.clear();
}

// Function declarations
String readCard();

void loop()
{
  lcd.clear();
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("card...");

  // Wait until a card is found
  while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  {
    delay(50);
  }
  
  // Read the card's UID and print it to the Serial Monitor
  String cardID = readCard();

  Serial.println("Card ID: " + cardID);
  lcd.clear();
  lcd.print("Card Found!");
  delay(1000);
  lcd.clear();
  

  // Send card ID to Google Apps Script
  HTTPClient http;

  // add code to get status

  boolean checkedIn = 0;

  if (checkedIn == 0)
  {
    lcd.print("Checked-in!");
    // add code to change checked in status on google sheets
  }
  else if (checkedIn == 1)
  {
    lcd.print("Checked-out!");
    // add code to change checked in status on google sheets
  }
  delay(1000);
}

String readCard()
{
  // Read the card's UID
  String cardID = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    cardID += String(rfid.uid.uidByte[i], HEX);
  }
  cardID.toUpperCase();

  // Stops the reader from instantly reading the same card
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  return cardID;
}
