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
RTC_DATA_ATTR bool localyn = 0;
DNSServer configServer;
const int refresh = GPIO_NUM_13;
const int changeMode = GPIO_NUM_14;
bool saved = false;

void centerText(String text);
void rightAlign(String text, int y);
void hSettings();
void hSave();
void sleepy();


// [BEGIN lopaka generated, and i edit]

static const unsigned char PROGMEM image_coud_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x0c,0x30,0x30,0x0c,0x40,0x02,0x40,0x02,0x40,0x02,0x23,0x84,0x1c,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static const unsigned char PROGMEM image_droplet_bits[] = {0x00,0x00,0x00,0x00,0x01,0x00,0x02,0x80,0x04,0x40,0x04,0x40,0x04,0x20,0x08,0x20,0x08,0x10,0x08,0x10,0x08,0x10,0x08,0x10,0x04,0x10,0x03,0xe0,0x00,0x00,0x00,0x00};

static const unsigned char PROGMEM image_flake_bits[] = {0x00,0x00,0x01,0x00,0x31,0x18,0x19,0x30,0x0d,0x60,0x07,0xc0,0x04,0x60,0x7d,0x7c,0x04,0x40,0x0f,0xc0,0x19,0x60,0x31,0x30,0x61,0x1c,0x01,0x06,0x00,0x00,0x00,0x00};

static const unsigned char PROGMEM image_thermo_bits[] = {0x00,0x00,0x00,0x00,0x01,0x80,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x04,0x20,0x08,0x10,0x08,0x10,0x08,0x10,0x04,0x20,0x03,0xc0,0x00,0x00,0x00,0x00};

void drawWeather(String time, float temp, float feels, int clud, float rain, bool snow, int rainChance, float rainTotal, float maxTemp, float minTemp) {
    display.fillScreen(GxEPD_WHITE);
    // line 1
    display.drawLine(0, 30, 295, 30, GxEPD_BLACK);
    // string 2
    display.setTextColor(GxEPD_BLACK);
    display.setTextWrap(false);
    display.setFont(midFont);
    display.setCursor(112, 22);
    display.print(time);
    // string 3
    display.setFont(thinFont);
    display.setCursor(22, 46);
    String temperature = String(temp,1) + "C" + "(" + String(feels,1) + "C)";
    display.print(temperature);
    // thermo
    display.drawBitmap(6, 34, image_thermo_bits, 16, 16, GxEPD_BLACK);
    // flake
    if (snow == true) {
      display.drawBitmap(217, 65, image_flake_bits, 16, 16, GxEPD_BLACK);
    }
    // droplet
    display.drawBitmap(6, 95, image_droplet_bits, 16, 16, GxEPD_BLACK);
    // coud
    display.drawBitmap(6, 65, image_coud_bits, 16, 16, GxEPD_BLACK);
    // string 8
    display.setFont(minFont);
    display.setCursor(6, 26);
    display.print("Current");
    // string 9
    display.setCursor(233, 25);
    display.print("Today");
    // string 10
    display.setFont(thinFont);
    String dayTemp = String(minTemp,1) + "C/" + String(maxTemp,1) + "C";
    rightAlign(dayTemp, 46);
    // string 11
    display.setCursor(22, 77);
    display.print(String(clud) + "%");
    // string 12
    if (snow == 1) {
      display.setCursor(233, 77);
      display.print("SNOW!");
    }
    // string 13
    display.setCursor(22, 108);
    display.print(String(rain,1));
    // string 14
    rightAlign(String(rainChance) + "% " + String(rainTotal) + "mm", 108);
    
    display.display();
}
// [END lopaka generated, i edited tho.]

void setup() {
  pinMode(refresh, INPUT_PULLUP);
  pinMode(changeMode, INPUT_PULLUP);
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  display.init(115200);
  display.setRotation(1);
  display.setFullWindow();
  display.setTextColor(GxEPD_BLACK);
  delay(500);
  if (digitalRead(refresh) == LOW && digitalRead(changeMode) == LOW) {
    LittleFS.begin(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspotName);
    centerText(WiFi.softAPIP().toString());
    configServer.start(53, "*", WiFi.softAPIP());
    server.on("/settings", HTTP_GET, hSettings);
    server.on("/save", HTTP_POST, hSave);
    server.serveStatic("/", LittleFS, "/index.html");
    server.onNotFound([]() {
      server.sendHeader("Location", "http://192.168.4.1/"); server.send(302);
    });
    while (digitalRead(refresh) == LOW || digitalRead(changeMode) == LOW) {
      delay(10);
    }
    server.begin();
    unsigned long current;
    current = millis();
    while((digitalRead(refresh) == HIGH || digitalRead(changeMode) == HIGH) && saved == false &&(configTimeout == 0 ? true : millis() - current <= configTimeout)) {
      configServer.processNextRequest();
      server.handleClient();
      delay(1);
    }
    centerText(saved ? "Saved! :O" : "Config Ended :/");
    delay(3000);
    while(digitalRead(refresh) == LOW || digitalRead(changeMode) == LOW) {
      delay(10);
    }
    esp_restart();
  }
  else if (digitalRead(refresh) == LOW && digitalRead(changeMode) == HIGH) {
    if (localyn == false) {
      settings.begin("weather", true);
      String ssid = settings.getString("ssid", "");
      String pass = settings.getString("pass", "");
      float lat = settings.getFloat("lat", defaultLat);
      float lon = settings.getFloat("lon", defaultLon);
      settings.end();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid.c_str(), pass.c_str());
      unsigned long current = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - current < 15000) {
        delay(200);
      }
      if (WiFi.status() != WL_CONNECTED) {
        centerText("Connection Failed D:");
        delay(15000);
        esp_restart();
      }
      String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(lat, 4) + "&longitude=" + String(lon,4) + "&daily=precipitation_probability_max,precipitation_sum,snowfall_sum,apparent_temperature_max,apparent_temperature_min&current=temperature_2m,apparent_temperature,cloud_cover,precipitation&timezone=auto&forecast_days=1";
      WiFiClientSecure client;
      client.setInsecure();
      HTTPClient http;
      http.begin(client,url);
      if (http.GET() == 200) {
        JsonDocument temp;
        DeserializationError parseCode = deserializeJson(temp, http.getString());
        if (parseCode == DeserializationError::Ok) {
          String time = temp["current"]["time"].as<String>().substring(11,16);
          float t = temp["current"]["temperature_2m"];
          float appTemp = temp["current"]["apparent_temperature"];
          int cloudPer = temp["current"]["cloud_cover"];
          float currPerc = temp["current"]["precipitation"];
          int percProb = temp["daily"]["precipitation_probability_max"][0];
          bool snowyn = (temp["daily"]["snowfall_sum"][0].as<float>() > 1 ? true : false);
          float totPerc = temp["daily"]["precipitation_sum"][0];
          float maxTemp = temp["daily"]["apparent_temperature_max"][0];
          float minTemp = temp["daily"]["apparent_temperature_min"][0];
          drawWeather(time,t,appTemp,cloudPer,currPerc,snowyn,percProb,totPerc,maxTemp,minTemp);
          http.end();
          localyn == true;
          sleepy();
        }
        else {
          centerText(parseCode.c_str());
          delay(15000);
          esp_restart();
        }
      }
      else {
        centerText("Server Connection Failed D:");
        delay(15000);
        esp_restart();
      }
    }
    else {

    }
  }
}

void loop(){}

void centerText(String text) {
  display.setFont(midFont);
  int16_t tbx, tby; 
  uint16_t tbw, tbh;
  display.getTextBounds(text,0,0, &tbx, &tby, &tbw, &tbh);
  int16_t x = (display.width() - tbw) / 2 - tbx;
  int16_t y = (display.height() - tbh) / 2 - tby;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x,y);
    display.print(text);
  }
  while(display.nextPage());
}

void rightAlign(String text, int y) {
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(text,0,0, &tbx,&tby, &tbw, &tbh);
  display.setCursor(display.width() - tbx - tbw - 8, y);
  display.print(text);
}

void hSettings() {
  settings.begin("weather", false);
  JsonDocument temp;
  temp["lat"] = settings.getFloat("lat", defaultLat);
  temp["lon"] = settings.getFloat("lon", defaultLon);
  temp["ssid"] = settings.getString("ssid", "");
  temp["refresh"] = settings.getULong64("refresh", autoRefresh) / 60000000ULL;
  settings.end();
  String out;
  serializeJson(temp, out);
  server.send(200, "application/json", out);
}

void hSave() {
  float lat = server.arg("lat").toFloat();
  float lon = server.arg("lon").toFloat();
  settings.begin("weather", false);
  settings.putFloat("lat", lat);
  settings.putFloat("lon", lon);
  settings.putString("ssid", server.arg("ssid"));
  settings.putULong64("refresh", server.arg("refresh").toInt() * 60000000ULL);
  if (server.arg("pass").length() > 0) {
    settings.putString("pass", server.arg("pass"));
  }
  settings.end();
  server.send(200, "text/plain", "Save success. Refreshing...");
  saved = true;
}

void sleepy() {
  display.hibernate();
  server.stop() ;
  configServer.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  rtc_gpio_pullup_en(GPIO_NUM_13);
  rtc_gpio_pullup_en(GPIO_NUM_14);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);
  esp_sleep_enable_ext1_wakeup(1ULL << 14, ESP_EXT1_WAKEUP_ALL_LOW);
  settings.begin("weather", true);
  uint64_t temp = settings.getULong64("refresh", autoRefresh);
  settings.end();
  if (temp > 0) {
    esp_sleep_enable_timer_wakeup(temp);
  }
  esp_deep_sleep_start();
}
