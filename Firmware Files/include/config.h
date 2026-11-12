#pragma once

#include <gfxfont.h>
#include <Fonts/FreeMonoBold9pt7b.h> // fonts. find list of options in https://github.com/adafruit/Adafruit-GFX-Library/tree/master/src/Fonts. If you change it you may need to test sizes.
const GFXfont* const minFont = &FreeMonoBold9pt7b; // copy your font choice without the .h, and put it after the &
#include <Fonts/FreeMono9pt7b.h>
const GFXfont* const thinFont = &FreeMono9pt7b;
#include <Fonts/FreeMonoBold12pt7b.h>
const GFXfont* const midFont = &FreeMonoBold12pt7b;
#include <Fonts/FreeMonoBold18pt7b.h>
const GFXfont* const maxFont = &FreeMonoBold18pt7b;


// Settings below can be edited in the future using config, only defaults here

constexpr uint64_t autoRefresh = 0; // 0 = no auto refresh. In Micro Seconds. 
constexpr const char* hotspotName = "WeatherSetup"; // name of hotspot for config
constexpr unsigned long configTimeout = 1000UL * 60 * 10; // Time before the config mode turns off. In milliseconds. 0 = no auto timeout

constexpr const float defaultLat = 49.193540f;
constexpr const float defaultLon = -123.179011f; // default longitude and latitudes. Default is a rough estimate of YVR's coordiates.
