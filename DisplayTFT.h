// DisplayTFT.h - Hỗ trợ màn hình TFT ST7789 2.0 inch + EC11 Rotary Encoder
// PHIÊN BẢN HOÀN CHỈNH - TƯƠNG THÍCH ESP32 (S3, C3, C5) VÀ ESP8266
// CÁCH 3: ĐỊNH NGHĨA TRỰC TIẾP TRONG CODE - KHÔNG CẦN USER_SETUP.H
#ifndef DISPLAY_TFT_H
#define DISPLAY_TFT_H

#include <SPI.h>
#include <RotaryEncoder.h>
#include "Settings.h"

// ==================== ĐỊNH NGHĨA CHÂN CHO TFT ====================
// QUAN TRỌNG: Định nghĩa TRƯỚC KHI include TFT_eSPI.h
#ifndef TFT_CS
  #define TFT_CS   13
#endif
#ifndef TFT_DC
  #define TFT_DC   9
#endif
#ifndef TFT_RST
  #define TFT_RST  10
#endif
#ifndef TFT_SCLK
  #define TFT_SCLK 12
#endif
#ifndef TFT_MOSI
  #define TFT_MOSI 11
#endif
#ifndef TFT_BLK
  #define TFT_BLK  46
#endif

// Định nghĩa driver cho ST7789
#define ST7789_DRIVER

// Kích thước màn hình
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Tần số SPI
#define SPI_FREQUENCY  40000000

// ==================== INCLUDE TFT_eSPI SAU KHI ĐỊNH NGHĨA ====================
#include <TFT_eSPI.h>

// ==================== Định nghĩa chân encoder ====================
#ifndef ENC_A
  #define ENC_A    1
#endif
#ifndef ENC_B
  #define ENC_B    2
#endif
#ifndef ENC_SW
  #define ENC_SW   3
#endif

#ifndef EXT_BUTTON
  #define EXT_BUTTON 4
#endif

// ==================== KHAI BÁO EXTERN ====================
extern TFT_eSPI tft;
extern RotaryEncoder encoder;

extern int tft_selected_page;
extern int last_encoder_pos;
extern unsigned long last_button_press;
extern bool button_pressed;
extern unsigned long last_display_update;
extern unsigned long last_ext_button_press;
extern bool ext_button_pressed;

extern String last_hashrate;
extern String last_accepted;
extern String last_total;
extern String last_uptime;
extern String last_node;
extern String last_difficulty;
extern String last_sharerate;
extern String last_ping;
extern String last_accept_rate;

// ==================== ĐỊNH NGHĨA HÀM ====================

void tft_setup() {
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    // ============ BACKLIGHT PWM - CHUẨN ESP32 CORE 3.0+ ============
    #if defined(ESP32)
        // Cách mới: ledcAttach(pin, freq, resolution) + ledcWrite(pin, duty)
        ledcAttach(TFT_BLK, 5000, 8);  // Pin, Frequency (Hz), Resolution (bits)
        ledcWrite(TFT_BLK, 200);        // Độ sáng 200/255 (~78%)
    #elif defined(ESP8266)
        // ESP8266 dùng analogWrite
        pinMode(TFT_BLK, OUTPUT);
        analogWrite(TFT_BLK, 200);
    #else
        // Fallback cho các board khác
        pinMode(TFT_BLK, OUTPUT);
        digitalWrite(TFT_BLK, HIGH);
    #endif
    // ================================================================
    
    // Khởi tạo encoder
    encoder.setPosition(0);
    pinMode(ENC_SW, INPUT_PULLUP);
    pinMode(EXT_BUTTON, INPUT_PULLUP);
    
    #if defined(SERIAL_PRINTING)
        Serial.println("TFT ST7789 initialized");
        Serial.println("Encoder: A=" + String(ENC_A) + ", B=" + String(ENC_B) + ", SW=" + String(ENC_SW));
        Serial.println("Extra button K0: GPIO" + String(EXT_BUTTON));
    #endif
}

void tft_display_boot() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(25, 40);
    tft.print("Duino");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("-Coin");
    
    tft.setTextSize(1);
    tft.setCursor(25, 85);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    #if defined(ESP32S3)
        tft.print("ESP32-S3 N8R16");
    #elif defined(ESP32C5)
        tft.print("ESP32-C5");
    #elif defined(ESP8266)
        tft.print("ESP8266");
    #else
        tft.print("ESP32");
    #endif
    
    tft.setCursor(25, 105);
    tft.print("v" + String(SOFTWARE_VERSION));
    
    tft.setCursor(25, 125);
    tft.print("Compiled: ");
    tft.print(__DATE__);
    
    tft.setCursor(25, 155);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("Connecting");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(" to WiFi...");
    
    tft.setTextSize(2);
}

void tft_display_info(String message) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawRect(5, 5, 230, 60, TFT_BLUE);
    tft.setCursor(10, 35);
    tft.print(message);
    tft.setTextSize(1);
    tft.setCursor(10, 85);
    tft.print("Please wait...");
    tft.setTextSize(2);
}

void tft_display_mining(String hashrate, String accepted, String total, 
                        String uptime, String node, String difficulty,
                        String sharerate, String ping, String accept_rate) {
    
    if (hashrate == last_hashrate && accepted == last_accepted && 
        total == last_total && uptime == last_uptime && 
        node == last_node && difficulty == last_difficulty &&
        sharerate == last_sharerate && ping == last_ping &&
        accept_rate == last_accept_rate && millis() - last_display_update < 500) {
        return;
    }
    
    last_hashrate = hashrate;
    last_accepted = accepted;
    last_total = total;
    last_uptime = uptime;
    last_node = node;
    last_difficulty = difficulty;
    last_sharerate = sharerate;
    last_ping = ping;
    last_accept_rate = accept_rate;
    last_display_update = millis();
    
    tft.fillScreen(TFT_BLACK);
    
    // Hashrate
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(10, 8);
    tft.print(hashrate);
    tft.setTextSize(2);
    tft.print(" kH/s");
    
    // Thanh Hashrate
    int bar_width = map(hashrate.toFloat(), 0, 200, 0, 220);
    if (bar_width > 220) bar_width = 220;
    if (bar_width < 0) bar_width = 0;
    tft.fillRect(10, 48, bar_width, 6, TFT_CYAN);
    tft.drawRect(10, 48, 220, 6, TFT_DARKGREY);
    
    // Shares
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 68);
    tft.print("Shares: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(accepted);
    tft.print("/");
    tft.print(total);
    tft.print(" (");
    tft.print(accept_rate);
    tft.print("%)");
    
    // Difficulty và Sharerate
    tft.setCursor(10, 98);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("Diff: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(difficulty);
    
    tft.setCursor(130, 98);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("Shr/s: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(sharerate);
    
    // Node và Ping
    tft.setCursor(10, 128);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.print("Node: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    String short_node = node;
    if (short_node.length() > 12) short_node = short_node.substring(0, 10) + "..";
    tft.print(short_node);
    
    tft.setCursor(160, 128);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.print("Ping: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(ping);
    tft.print("ms");
    
    // Uptime
    tft.setCursor(10, 158);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.print("Uptime: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(uptime);
    
    // Rig ID và IP
    tft.setTextSize(1);
    tft.setCursor(10, 188);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.print("Rig: ");
    tft.print(RIG_IDENTIFIER);
    
    #ifdef USE_LAN
        String ip = ETH.localIP().toString();
    #else
        String ip = WiFi.localIP().toString();
    #endif
    tft.setCursor(10, 203);
    tft.print("IP: ");
    tft.print(ip);
    
    tft.setCursor(170, 203);
    tft.print("v");
    tft.print(SOFTWARE_VERSION);
    
    tft.setTextSize(2);
}

void tft_display_system_info() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("System Info");
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    tft.setCursor(10, 40);
    tft.print("Free Heap: ");
    tft.print(ESP.getFreeHeap());
    tft.println(" bytes");
    
    tft.setCursor(10, 60);
    tft.print("CPU Freq: ");
    #if defined(ESP8266)
        tft.print(ESP.getCpuFreqMHz());
    #else
        tft.print(getCpuFrequencyMhz());
    #endif
    tft.println(" MHz");
    
    tft.setCursor(10, 80);
    #if defined(ESP8266)
        tft.print("Chip: ESP8266");
    #elif defined(ESP32C5)
        tft.print("Chip: ESP32-C5");
    #elif defined(ESP32S3)
        tft.print("Chip: ESP32-S3");
    #else
        tft.print("Chip: ESP32");
    #endif
    
    tft.setCursor(10, 100);
    tft.print("Flash: ");
    tft.print(ESP.getFlashChipSize() / (1024 * 1024));
    tft.println(" MB");
    
    #ifndef USE_LAN
        int rssi = WiFi.RSSI();
        tft.setCursor(10, 120);
        tft.print("WiFi RSSI: ");
        tft.print(rssi);
        tft.println(" dBm");
        
        int bars = 0;
        if (rssi > -50) bars = 4;
        else if (rssi > -60) bars = 3;
        else if (rssi > -70) bars = 2;
        else if (rssi > -80) bars = 1;
        tft.fillRect(100, 118, 40, 6, TFT_DARKGREY);
        tft.fillRect(100, 118, bars * 10, 6, TFT_GREEN);
    #endif
    
    long millisecs = millis();
    int uptime_days = int(millisecs / (1000 * 60 * 60 * 24));
    int uptime_hours = int((millisecs / (1000 * 60 * 60)) % 24);
    int uptime_mins = int((millisecs / (1000 * 60)) % 60);
    tft.setCursor(10, 150);
    tft.print("Uptime: ");
    tft.print(uptime_days);
    tft.print("d ");
    tft.print(uptime_hours);
    tft.print("h ");
    tft.print(uptime_mins);
    tft.println("m");
    
    tft.setTextSize(2);
}

void tft_display_stats() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("Mining Stats");
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    tft.setCursor(10, 40);
    tft.print("Total Shares: ");
    tft.print(share_count);
    
    tft.setCursor(10, 60);
    tft.print("Accepted: ");
    tft.print(accepted_share_count);
    
    float accept_rate = (share_count > 0) ? (accepted_share_count * 100.0 / share_count) : 0;
    tft.setCursor(10, 80);
    tft.print("Acceptance: ");
    tft.print(accept_rate, 1);
    tft.println("%");
    
    tft.setCursor(10, 100);
    tft.print("Difficulty: ");
    tft.print(difficulty / 100);
    
    tft.setCursor(10, 120);
    tft.print("Node: ");
    tft.print(node_id);
    
    tft.setCursor(10, 140);
    tft.print("Avg Ping: ");
    tft.print(ping);
    tft.println(" ms");
    
    float total_hashrate = (hashrate + hashrate_core_two) / 1000.0;
    tft.setCursor(10, 160);
    tft.print("Total HR: ");
    tft.print(total_hashrate, 1);
    tft.println(" kH/s");
    
    tft.setTextSize(2);
}

void tft_read_encoder() {
    encoder.tick();
    int newPos = encoder.getPosition() / 4;
    
    if (newPos != last_encoder_pos) {
        int delta = newPos - last_encoder_pos;
        if (delta > 0) {
            tft_selected_page++;
        } else {
            tft_selected_page--;
        }
        if (tft_selected_page < 0) tft_selected_page = 2;
        if (tft_selected_page > 2) tft_selected_page = 0;
        last_encoder_pos = newPos;
        
        #if defined(SERIAL_PRINTING)
            Serial.println("Page: " + String(tft_selected_page));
        #endif
        
        if (tft_selected_page == 1) {
            tft_display_system_info();
        } else if (tft_selected_page == 2) {
            tft_display_stats();
        } else {
            last_hashrate = "";
        }
    }
    
    // Xử lý nút nhấn encoder (SW)
    if (digitalRead(ENC_SW) == LOW && !button_pressed && millis() - last_button_press > 200) {
        button_pressed = true;
        last_button_press = millis();
        
        if (tft_selected_page != 0) {
            tft_selected_page = 0;
            last_encoder_pos = 0;
            encoder.setPosition(0);
            last_hashrate = "";
        }
    }
    
    if (digitalRead(ENC_SW) == HIGH && button_pressed) {
        button_pressed = false;
    }
    
    // Xử lý nút phụ K0
    if (digitalRead(EXT_BUTTON) == LOW && !ext_button_pressed && millis() - last_ext_button_press > 200) {
        ext_button_pressed = true;
        last_ext_button_press = millis();
        
        #if defined(SERIAL_PRINTING)
            Serial.println("Extra button K0 pressed!");
        #endif
        
        if (tft_selected_page != 0) {
            tft_selected_page = 0;
            last_encoder_pos = 0;
            encoder.setPosition(0);
            last_hashrate = "";
        } else {
            tft_display_info("K0 Pressed!");
            delay(1000);
        }
    }
    
    if (digitalRead(EXT_BUTTON) == HIGH && ext_button_pressed) {
        ext_button_pressed = false;
    }
}

#endif  // DISPLAY_TFT_H
