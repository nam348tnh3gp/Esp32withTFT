// DisplayTFT.h - Hỗ trợ màn hình TFT ST7789 2.0 inch + EC11 Rotary Encoder
// PHIÊN BẢN ĐÃ SỬA LỖI REDEFINITION
#ifndef DISPLAY_TFT_H
#define DISPLAY_TFT_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include <RotaryEncoder.h>

// ==================== Định nghĩa chân cắm ====================
#ifndef TFT_CS
  #define TFT_CS   8
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

#ifndef ENC_A
  #define ENC_A    2
#endif
#ifndef ENC_B
  #define ENC_B    3
#endif
#ifndef ENC_SW
  #define ENC_SW   1
#endif

// ==================== KHAI BÁO EXTERN (định nghĩa trong .ino) ====================
extern TFT_eSPI tft;
extern RotaryEncoder encoder;

extern int tft_selected_page;
extern int last_encoder_pos;
extern unsigned long last_button_press;
extern bool button_pressed;
extern unsigned long last_display_update;

extern String last_hashrate;
extern String last_accepted;
extern String last_total;
extern String last_uptime;
extern String last_node;
extern String last_difficulty;
extern String last_sharerate;
extern String last_ping;
extern String last_accept_rate;

// ==================== KHAI BÁO HÀM ====================
void tft_setup();
void tft_display_boot();
void tft_display_info(String message);
void tft_display_mining(String hashrate, String accepted, String total, 
                        String uptime, String node, String difficulty,
                        String sharerate, String ping, String accept_rate);
void tft_display_system_info();
void tft_display_stats();
void tft_read_encoder();

#endif  // DISPLAY_TFT_H
