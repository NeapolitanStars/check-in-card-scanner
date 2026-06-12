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
int rs_pin = 14; // For LCD screen
int E_pin = 27;  // For LCD screen
int D4_pin = 26; // For LCD screen
int D5_pin = 25; // For LCD screen
int D6_pin = 33; // For LCD screen
int D7_pin = 32; // For LCD screen

// URL of the Google Apps Script
String scriptURL = "https://script.google.com/macros/s/AKfycbwSb80KNG5SlXwEkUxk2ShaanLo-9TH9o9o5RNbp_TRhhH38lSQqRorXvlZqClvzm1bkA/exec";
// Initialize LCD screen
LiquidCrystal lcd(rs_pin, E_pin, D4_pin, D5_pin, D6_pin, D7_pin); // RS, E, D4, D5, D6, D7

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

  // Turn on the built-in LED and print to Serial Monitor and LCD screen
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
  lcd.print("Processing...");
  delay(1000);
  lcd.clear();

  // Send card ID to Google Apps Script
  HTTPClient http;
  String url = scriptURL + "?cardID=" + cardID;

  http.begin(url);
  // Force the HTTP client to follow redirects, so it can properly handle the response from the Google Apps Script
  http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
  // Send the GET request and get the response
  int httpResponseCode = http.GET();
  String response = http.getString();
  response.trim();

  // Print the response to the Serial Monitor and LCD screen
  if (response == "CHECKED IN")
  {
    lcd.print("Checked-in!");
  }
  else if (response == "CHECKED OUT")
  {
    lcd.print("Checked-out!");
  }
  else
  {
    lcd.print("Error!");
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
