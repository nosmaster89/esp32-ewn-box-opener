/*
 * ------------------------------------------------------------------------
 * Project Name: esp32-ewn-box-opener
 * Description: Esp32 port of the Box Opener client for mining EWN tokens.
 * Author: Crey
 * Repository: https://github.com/cr3you/esp32-ewn-box-opener/
 * Date: 2024.10.04
 * Version: 1.0.1
 * License: MIT
 * ------------------------------------------------------------------------
 */

#include "bip39/bip39.h"
#include <Arduino.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
#define DELAYVAL 500
#define BRIGHTNESS 120

#define PIN 48 // 48 on the esp32s3 usbC devboard
//=====wifi setup
const char *ssid = "SSID";         // <---------------------- SET THIS !!!
const char *password = "PASSWORD"; // <-------------- SET THIS !!!
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//=====Box Opener client setup
const char *apiUrl = "https://api.erwin.lol/"; // mainnet
// const char *apiUrl = "https://devnet-api.erwin.lol/"; // devnet
const char *apiKey = ""; // <---------------------- SET THIS !!!

const int numGuesses = 50;
String mnemonics[numGuesses]; // bip39 mnemonic table
int sleepTime = 10000;        // default sleep time in ms

void blinkPixels(int color); // Forward declaration
void lightPixels(int color); // Forward declaration
void setup()
{
  Serial.begin(115200);
  pixels.begin();
  // set all pixels to red
  pixels.clear();

  lightPixels(pixels.Color(BRIGHTNESS, 0, 0));
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  // blink the pixels to green
  blinkPixels(pixels.Color(0, 0, BRIGHTNESS));

  Serial.println();
  Serial.println(WiFi.localIP()); // print local IP

  Serial.printf("===============\n");
  Serial.printf("Box-opener started\n");

  pixels.clear();
}

//===== generate table of 50 bip39 12-word mnemonics
void generateMnemonics(String *mnemonics)
{
  for (int i = 0; i < numGuesses; i++)
  {
    std::vector<uint8_t> entropy(16); // 128-bit entropy -> 12 words
    for (size_t j = 0; j < entropy.size(); j++)
    {
      entropy[j] = esp_random() & 0xFF; // extract 1 random byte
    }
    BIP39::word_list mnemonic = BIP39::create_mnemonic(entropy);
    std::string mnemonicStr = mnemonic.to_string();
    mnemonics[i] = String(mnemonicStr.c_str());
    // Serial.printf("Generated mnemonic: %s\n", mnemonics[i].c_str());
  }
}
void lightPixels(int color)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
void blinkPixels(int color)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(DELAYVAL);
    pixels.clear();
    pixels.show();
  }
}
//===== submit mnemonics to Oracle
bool submitGuesses(String *mnemonics, const String &apiUrl, const String &apiKey)
{
  bool ret = false;
  DynamicJsonDocument jsonDoc(8192); // max size of the json output, to verify! (4kB was not enough)

  for (int i = 0; i < numGuesses; i++)
  {
    jsonDoc.add(mnemonics[i]);
  }

  Serial.printf("🔑️ Generated %u guesses\n", numGuesses);
  Serial.printf("➡️ Submitting to oracle\n");

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  HTTPClient http;
  http.begin(apiUrl + "/submit_guesses");
  http.addHeader("x-api-key", apiKey);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(15000); // increase default 5s to 15s

  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    if (httpResponseCode == 202)
    {
      Serial.println("✅ Guesses accepted");
      // make the pixels green
      blinkPixels(pixels.Color(0, BRIGHTNESS, 0));
      ret = false;
    }
    else if (httpResponseCode == 404) // "Closed Box Not Found"
    {
      Serial.printf("❌ Guesses rejected (%d): %s\n", httpResponseCode, response.c_str());
      ret = false;
      blinkPixels(pixels.Color(102, 0, 255));
    }
    else // other errors
    {
      Serial.printf("❌ Guesses rejected (%d): %s\n", httpResponseCode, response.c_str());
      ret = true;
      blinkPixels(pixels.Color(BRIGHTNESS, 0, 0));
    }
  }
  else // even more other errors :V maybe do a reconnect?
  {
    Serial.printf("❌ Error in HTTP request: %s\n", http.errorToString(httpResponseCode).c_str());
    ret = true;
    blinkPixels(pixels.Color(BRIGHTNESS, 0, 0));
  }

  http.end();
  return ret;
}

//====== main loop ====
void loop()
{
  //--- reconnect wifi if it is not connected by some reason
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WiFi disconnected, trying to reconnect..\n");
    WiFi.disconnect();
    WiFi.reconnect();
  }

  Serial.println("⚙️ Generating guesses...");

  generateMnemonics(mnemonics);

  bool rateLimited = submitGuesses(mnemonics, apiUrl, apiKey);

  if (rateLimited)
  {
    sleepTime += 10000;
  }
  else
  {
    sleepTime -= 1000;
  }

  if (sleepTime < 10000)
  {
    sleepTime = 10000;
  }
  if (sleepTime > 60000) // if sleep for more than a minute limit it to one minute
  {
    sleepTime = 60000;
  }

  Serial.printf("waiting %is for next batch...\n", sleepTime / 1000);
  pixels.clear();
  delay(sleepTime - DELAYVAL);
}
