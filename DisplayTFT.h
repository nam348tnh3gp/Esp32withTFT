// Settings.h - Cho ESP32-C5
#ifndef SETTINGS_H
#define SETTINGS_H

// ==================== THÔNG TIN CÀI ĐẶT ==================== //
// Tài khoản Duino-Coin của bạn
extern char *DUCO_USER = "your_username";
// Mining key (nếu có, không thì để "None")
extern char *MINER_KEY = "None";
// Tên rig (tùy chỉnh)
extern char *RIG_IDENTIFIER = "ESP32-C5";
// WiFi của bạn
extern const char SSID[] = "your_wifi";
extern const char PASSWORD[] = "your_password";
// ============================================================ //

// ==================== TÙY CHỌN NÂNG CAO ==================== //
// Bật web dashboard (truy cập qua IP)
#define WEB_DASHBOARD

// Bật đèn LED nhấp nháy
#define LED_BLINKING

// In ra Serial (có thể tắt để tăng hiệu suất nhẹ)
#define SERIAL_PRINTING

// Tốc độ Serial
#define SERIAL_BAUDRATE 115200

// Vô hiệu hóa brownout detector
#define DISABLE_BROWNOUT

// Bật WiFiManager (cấu hình qua web lần đầu)
// #define CAPTIVE_PORTAL

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

#define SOFTWARE_VERSION "4.3-C5"

extern unsigned int hashrate = 0;
extern unsigned int hashrate_core_two = 0;
extern unsigned int difficulty = 0;
extern unsigned long share_count = 0;
extern unsigned long accepted_share_count = 0;
extern String node_id = "";
extern String WALLET_ID = "";
extern unsigned int ping = 0;

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
