// Settings.h - Cho ESP32-C5 với TFT ST7789
#ifndef SETTINGS_H
#define SETTINGS_H

// ==================== THÔNG TIN CÀI ĐẶT ==================== //
extern char *DUCO_USER = "your_username";
extern char *MINER_KEY = "None";
extern char *RIG_IDENTIFIER = "ESP32-C5-TFT";
extern const char SSID[] = "your_wifi";
extern const char PASSWORD[] = "your_password";
// ============================================================ //

// ==================== TÙY CHỌN NÂNG CAO ==================== //
#define WEB_DASHBOARD
#define LED_BLINKING
#define SERIAL_PRINTING
#define SERIAL_BAUDRATE 115200
#define DISABLE_BROWNOUT
// #define CAPTIVE_PORTAL
// ============================================================ //

// ==================== MÀN HÌNH TFT ST7789 ==================== //
#define DISPLAY_TFT_ST7789

// Chân kết nối TFT SPI
#define TFT_CS   8
#define TFT_DC   9
#define TFT_RST  10
#define TFT_SCLK 12
#define TFT_MOSI 11
#define TFT_BLK  46

// Chân EC11 Rotary Encoder
#define ENC_A    2
#define ENC_B    3
#define ENC_SW   1
// ============================================================ //

// ==================== CẢM BIẾN (TÙY CHỌN) ==================== //
// #define USE_INTERNAL_SENSOR
// #define USE_DS18B20
// #define USE_DHT
// ============================================================ //

// ==================== ĐỊNH NGHĨA HỆ THỐNG ==================== //
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

#define BLINK_SETUP_COMPLETE 2
#define BLINK_CLIENT_CONNECT 5
#define SOFTWARE_VERSION "4.3-C5-TFT"

extern unsigned int hashrate = 0;
extern unsigned int hashrate_core_two = 0;
extern unsigned int difficulty = 0;
extern unsigned long share_count = 0;
extern unsigned long accepted_share_count = 0;
extern String node_id = "";
extern String WALLET_ID = "";
extern unsigned int ping = 0;

// Biến cho TFT
extern int tft_selected_page = 0;

#if defined(USE_INTERNAL_SENSOR)
  #include "driver/temp_sensor.h"
#endif

#if defined(USE_DS18B20)
  #include <OneWire.h>
  #include <DallasTemperature.h>
  const int DSPIN = 12;
  OneWire oneWire(DSPIN);
  DallasTemperature sensors(&oneWire);
#endif

#if defined(USE_DHT)
  #include <DHT.h>
  #define DHTPIN 12
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
#endif

#endif
