#define ENABLE_GxEPD2_GFX 0

#include <Arduino.h> // for arduino stuffs
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
#include "config.h" // config stuff

GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(5,4,2,12));
Preferences settings;
WebServer server(80);
RTC_DATA_ATTR int mode = 0;
DNSServer configServer;
const int refresh = 13;
const int changeMode = 14;

void setup() {
  pinMode(refresh, INPUT_PULLUP);
  pinMode(changeMode, INPUT_PULLUP);
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  display.init(115200);
  display.setRotation(1);
  display.setFont(font);
  display.setTextColor(GxEPD_BLACK);
  delay(500);
  if (digitalRead(refresh) == LOW && digitalRead(changeMode) == LOW) {
    LittleFS.begin(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspotName);
    int16_t tbx, tby; 
    uint16_t tbw, tbh;
    display.getTextBounds(WiFi.softAPIP().toString(),0,0, &tbx, &tby, &tbw, &tbh);
    int16_t x = (display.width() - tbw) / 2 - tbx;
    int16_t y = (display.height() - tbh) / 2 - tby;
    display.setFullWindow();
    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(x,y);
      display.print(WiFi.softAPIP());
    }
    while(display.nextPage());
    configServer.start(53, "*", WiFi.softAPIP());
   
  }
}

void loop(){}
