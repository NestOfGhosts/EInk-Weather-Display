#pragma once

#include <gfxfont.h>
#include <Fonts/FreeMonoBold9pt7b.h> // fonts. find list of options in https://github.com/adafruit/Adafruit-GFX-Library/tree/master/src/Fonts. If you change it you may need to test sizes.
const GFXfont* const font = &FreeMonoBold9pt7b; // copy your font choice without the .h, and put it after the &

constexpr uint64_t refreshMinutes = 0; //0 = no auto refresh.
constexpr const char* hotspotName = "WeatherSetup"; // name of hotspot for config

constexpr const float defaultLat = 49.193540f;
constexpr const float defaultLon = -123.179011f;

