#define Enable_GxEPD2_GFX 0

#include <Arduino.h> // for arduino stuffs
#include "config.h" // config stuff
#include "driver/rtc_io.h" // Deepsleep stuff
#include <WiFi.h> // to connect to wifi
#include <WebServer.h> // to host config website
#include <DNSServer.h> // to make the hotspot for config website
#include <LittleFS.h> // file system for config website
#include <Preferences.h> // To save the settings
#include <HTTPClient.h> // Lets arduino read website info
#include <WiFiClientSecure.h> // Lets arduino read http*S* info
#include <ArduinoJson.h> // Parses(reads) json
#include <GxEPD2_BW.h> // Black & White epaper library

GxEPD2_GW<GxEPD2_290_BS, GxEPD290::Height> display(GxEPD2_290_BS(5,4,2,12));
Preferences settings;
WebServer server(80);
RTC_DATA_ATTR int mode = 0;
DNSServer configServer;
const int refresh = 13;
const int changeMode = 14;
void display();

void setup {
  pinMode(refresh, INPUT_PULLUP);
  pinMode(changeMode, INPUT_PULLUP);
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  delay(500)
  if (digitalRead(refresh) == LOW && digitalRead(changeMode) == LOW) {
    LittleFS.begin(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspotName);
    
  }
}
