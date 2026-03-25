// Settings.h - Cho ESP32-S3 N8R16 với TFT ST7789 + EC11
#ifndef SETTINGS_H
#define SETTINGS_H

// ==================== THÔNG TIN CÀI ĐẶT ==================== //
// Đổi thành username Duino-Coin của bạn
extern char *DUCO_USER = "your_username";
// Mining key (nếu có, không thì để "None")
extern char *MINER_KEY = "None";
// Tên rig (tùy chỉnh)
extern char *RIG_IDENTIFIER = "ESP32-S3-TFT";
// WiFi của bạn
extern const char SSID[] = "your_wifi";
extern const char PASSWORD[] = "your_password";
// ============================================================ //

// ==================== TÙY CHỌN NÂNG CAO ==================== //
// Bật web dashboard (truy cập qua IP)
#define WEB_DASHBOARD

// Bật đèn LED nhấp nháy
#define LED_BLINKING

// In ra Serial (có thể tắt để tăng hiệu suất)
#define SERIAL_PRINTING

// Tốc độ Serial
#define SERIAL_BAUDRATE 115200

// Vô hiệu hóa brownout detector
#define DISABLE_BROWNOUT

// Bật WiFiManager (cấu hình qua web lần đầu)
// #define CAPTIVE_PORTAL
// ============================================================ //

// ==================== MÀN HÌNH TFT ST7789 ==================== //
#define DISPLAY_TFT_ST7789

// Chân kết nối TFT SPI (theo DisplayTFT.h)
#define TFT_CS   13   // Chip Select
#define TFT_DC   9    // Data/Command
#define TFT_RST  10   // Reset
#define TFT_SCLK 12   // SPI Clock
#define TFT_MOSI 11   // SPI Data
#define TFT_BLK  46   // Backlight (PWM)

// Chân EC11 Rotary Encoder (theo DisplayTFT.h)
#define ENC_A    1    // Kênh A
#define ENC_B    2    // Kênh B
#define ENC_SW   3    // Nút nhấn

// Nút bấm phụ K0 (nếu có)
#define EXT_BUTTON 4
// ============================================================ //

// ==================== CẢM BIẾN (TÙY CHỌN) ==================== //
// Bật cảm biến nhiệt độ nội (ESP32-S3 có hỗ trợ)
// #define USE_INTERNAL_SENSOR

// Bật cảm biến DS18B20
// #define USE_DS18B20

// Bật cảm biến DHT11/DHT22
// #define USE_DHT
// ============================================================ //

// ==================== ĐỊNH NGHĨA HỆ THỐNG ==================== //
// LED_BUILTIN cho ESP32-S3 (thường là GPIO2 hoặc GPIO48 tùy board)
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

#define BLINK_SETUP_COMPLETE 2
#define BLINK_CLIENT_CONNECT 5
#define SOFTWARE_VERSION "4.3-S3-TFT"

// Biến toàn cục cho mining
extern unsigned int hashrate = 0;
extern unsigned int hashrate_core_two = 0;
extern unsigned int difficulty = 0;
extern unsigned long share_count = 0;
extern unsigned long accepted_share_count = 0;
extern String node_id = "";
extern String WALLET_ID = "";
extern unsigned int ping = 0;

// Biến cho TFT (sẽ được định nghĩa trong DisplayTFT.h)
extern int tft_selected_page;

// ==================== CẢM BIẾN (KHÔNG SỬA) ==================== //
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

#endif  // SETTINGS_H
