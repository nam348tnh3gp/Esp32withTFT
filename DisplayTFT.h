// DisplayTFT.h - Hỗ trợ màn hình TFT ST7789 2.0 inch + EC11 Rotary Encoder
#ifndef DISPLAY_TFT_H
#define DISPLAY_TFT_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include <RotaryEncoder.h>

// ==================== Định nghĩa chân cắm ====================
// TFT SPI - Theo sơ đồ đã xác định
#define TFT_CS   8
#define TFT_DC   9
#define TFT_RST  10
#define TFT_SCLK 12
#define TFT_MOSI 11
#define TFT_BLK  46

// EC11 Rotary Encoder
#define ENC_A    2
#define ENC_B    3
#define ENC_SW   1

// ==================== Khởi tạo đối tượng ====================
TFT_eSPI tft = TFT_eSPI();
RotaryEncoder encoder(ENC_A, ENC_B, RotaryEncoder::LatchMode::TWO03);

// Biến lưu trạng thái
int selected_page = 0;
int last_encoder_pos = 0;
unsigned long last_button_press = 0;
bool button_pressed = false;
unsigned long last_display_update = 0;

// Cache dữ liệu hiển thị
String last_hashrate = "";
String last_accepted = "";
String last_total = "";
String last_uptime = "";
String last_node = "";
String last_difficulty = "";
String last_sharerate = "";
String last_ping = "";
String last_accept_rate = "";

// ==================== Cấu hình màn hình ====================
void tft_setup() {
    // Khởi tạo SPI
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    
    // Khởi tạo màn hình
    tft.init();
    tft.setRotation(1);  // Xoay 90 độ
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    // Cấu hình backlight PWM
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BLK, 0);
    ledcWrite(0, 200);  // Độ sáng ~80%
    
    // Khởi tạo encoder
    encoder.setPosition(0);
    pinMode(ENC_SW, INPUT_PULLUP);
    
    #if defined(SERIAL_PRINTING)
        Serial.println("TFT ST7789 initialized");
        Serial.println("Encoder initialized on pins A=" + String(ENC_A) + ", B=" + String(ENC_B) + ", SW=" + String(ENC_SW));
    #endif
}

// ==================== Màn hình khởi động ====================
void tft_display_boot() {
    tft.fillScreen(TFT_BLACK);
    
    // Logo Duino-Coin
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(25, 40);
    tft.print("Duino");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("-Coin");
    
    // Chip info
    tft.setTextSize(1);
    tft.setCursor(25, 85);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print("ESP32-S3 N8R16");
    
    tft.setCursor(25, 105);
    tft.print("v" + String(SOFTWARE_VERSION));
    
    tft.setCursor(25, 125);
    tft.print("Compiled: ");
    tft.print(__DATE__);
    
    // WiFi / Ethernet indicator
    tft.setCursor(25, 155);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("Connecting");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(" to WiFi...");
    
    tft.setTextSize(2);
    
    #if defined(SERIAL_PRINTING)
        Serial.println("Boot screen displayed");
    #endif
}

// ==================== Màn hình thông tin trạng thái ====================
void tft_display_info(String message) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    // Vẽ khung
    tft.drawRect(5, 5, 230, 60, TFT_BLUE);
    
    tft.setCursor(10, 35);
    tft.print(message);
    
    tft.setTextSize(1);
    tft.setCursor(10, 85);
    tft.print("Please wait...");
    
    tft.setTextSize(2);
    
    #if defined(SERIAL_PRINTING)
        Serial.println("Info screen: " + message);
    #endif
}

// ==================== Hiển thị giao diện đào chính ====================
void tft_display_mining(String hashrate, String accepted, String total, 
                        String uptime, String node, String difficulty,
                        String sharerate, String ping, String accept_rate) {
    
    // Chỉ cập nhật khi dữ liệu thay đổi (giảm flicker)
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
    
    // === Dòng 1: Hashrate lớn ===
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(10, 8);
    tft.print(hashrate);
    tft.setTextSize(2);
    tft.print(" kH/s");
    
    // Thanh Hashrate (tương ứng với tốc độ)
    int bar_width = map(hashrate.toFloat(), 0, 200, 0, 220);
    if (bar_width > 220) bar_width = 220;
    if (bar_width < 0) bar_width = 0;
    tft.fillRect(10, 48, bar_width, 6, TFT_CYAN);
    tft.drawRect(10, 48, 220, 6, TFT_DARKGREY);
    
    // === Dòng 2: Shares ===
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
    
    // === Dòng 3: Difficulty và Sharerate ===
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
    
    // === Dòng 4: Node và Ping ===
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
    
    // === Dòng 5: Uptime ===
    tft.setCursor(10, 158);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.print("Uptime: ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(uptime);
    
    // === Dòng 6: Rig ID và IP ===
    tft.setTextSize(1);
    tft.setCursor(10, 188);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.print("Rig: ");
    tft.print(RIG_IDENTIFIER);
    
    // Hiển thị IP
    #ifdef USE_LAN
        String ip = ETH.localIP().toString();
    #else
        String ip = WiFi.localIP().toString();
    #endif
    tft.setCursor(10, 203);
    tft.print("IP: ");
    tft.print(ip);
    
    // Version ở góc phải
    tft.setCursor(170, 203);
    tft.print("v");
    tft.print(SOFTWARE_VERSION);
    
    tft.setTextSize(2);
}

// ==================== Màn hình thông tin hệ thống (trang 2) ====================
void tft_display_system_info() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("System Info");
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    // Free RAM
    tft.setCursor(10, 40);
    tft.print("Free Heap: ");
    tft.print(ESP.getFreeHeap());
    tft.println(" bytes");
    
    // CPU Frequency
    tft.setCursor(10, 60);
    tft.print("CPU Freq: ");
    tft.print(getCpuFrequencyMhz());
    tft.println(" MHz");
    
    // Chip info
    tft.setCursor(10, 80);
    tft.print("Chip Model: ESP32-S3");
    
    // Flash size
    tft.setCursor(10, 100);
    tft.print("Flash: ");
    tft.print(ESP.getFlashChipSize() / (1024 * 1024));
    tft.println(" MB");
    
    // Temperature (if available)
    #if defined(USE_INTERNAL_SENSOR)
        float temp = 0;
        temp_sensor_read_celsius(&temp);
        tft.setCursor(10, 120);
        tft.print("CPU Temp: ");
        tft.print(temp, 1);
        tft.println(" *C");
    #endif
    
    // WiFi RSSI
    #ifndef USE_LAN
        tft.setCursor(10, 140);
        tft.print("WiFi RSSI: ");
        tft.print(WiFi.RSSI());
        tft.println(" dBm");
        
        // WiFi strength bar
        int rssi = WiFi.RSSI();
        int bars = 0;
        if (rssi > -50) bars = 4;
        else if (rssi > -60) bars = 3;
        else if (rssi > -70) bars = 2;
        else if (rssi > -80) bars = 1;
        else bars = 0;
        
        tft.fillRect(100, 138, 40, 6, TFT_DARKGREY);
        tft.fillRect(100, 138, bars * 10, 6, TFT_GREEN);
    #endif
    
    // Uptime
    long millisecs = millis();
    int uptime_days = int(millisecs / (1000 * 60 * 60 * 24));
    int uptime_hours = int((millisecs / (1000 * 60 * 60)) % 24);
    int uptime_mins = int((millisecs / (1000 * 60)) % 60);
    tft.setCursor(10, 170);
    tft.print("Uptime: ");
    tft.print(uptime_days);
    tft.print("d ");
    tft.print(uptime_hours);
    tft.print("h ");
    tft.print(uptime_mins);
    tft.println("m");
    
    tft.setTextSize(2);
}

// ==================== Màn hình thống kê đào (trang 3) ====================
void tft_display_stats() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("Mining Stats");
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    // Total shares
    tft.setCursor(10, 40);
    tft.print("Total Shares: ");
    tft.print(share_count);
    
    // Accepted shares
    tft.setCursor(10, 60);
    tft.print("Accepted: ");
    tft.print(accepted_share_count);
    
    // Acceptance rate
    float accept_rate = (share_count > 0) ? (accepted_share_count * 100.0 / share_count) : 0;
    tft.setCursor(10, 80);
    tft.print("Acceptance: ");
    tft.print(accept_rate, 1);
    tft.println("%");
    
    // Current difficulty
    tft.setCursor(10, 100);
    tft.print("Difficulty: ");
    tft.print(difficulty / 100);
    
    // Current node
    tft.setCursor(10, 120);
    tft.print("Node: ");
    tft.print(node_id);
    
    // Average ping
    tft.setCursor(10, 140);
    tft.print("Avg Ping: ");
    tft.print(ping);
    tft.println(" ms");
    
    // Hashrate (both cores if dual)
    float total_hashrate = (hashrate + hashrate_core_two) / 1000.0;
    tft.setCursor(10, 160);
    tft.print("Total HR: ");
    tft.print(total_hashrate, 1);
    tft.println(" kH/s");
    
    tft.setTextSize(2);
}

// ==================== Đọc encoder và xử lý ====================
void tft_read_encoder() {
    encoder.tick();
    int newPos = encoder.getPosition() / 4;  // Giảm độ nhạy
    
    // Xoay encoder
    if (newPos != last_encoder_pos) {
        int delta = newPos - last_encoder_pos;
        if (delta > 0) {
            selected_page++;
        } else {
            selected_page--;
        }
        if (selected_page < 0) selected_page = 2;
        if (selected_page > 2) selected_page = 0;
        last_encoder_pos = newPos;
        
        #if defined(SERIAL_PRINTING)
            Serial.println("Encoder: page " + String(selected_page));
        #endif
        
        // Cập nhật hiển thị theo trang
        if (selected_page == 0) {
            // Trang chính sẽ được cập nhật lại ở lần gọi tft_display_mining tiếp theo
            last_hashrate = "";  // Force refresh
        } else if (selected_page == 1) {
            tft_display_system_info();
        } else if (selected_page == 2) {
            tft_display_stats();
        }
    }
    
    // Nút nhấn encoder
    if (digitalRead(ENC_SW) == LOW && !button_pressed && millis() - last_button_press > 200) {
        button_pressed = true;
        last_button_press = millis();
        
        #if defined(SERIAL_PRINTING)
            Serial.println("Encoder button pressed! Page: " + String(selected_page));
        #endif
        
        // Reset về trang chính khi nhấn nút
        if (selected_page != 0) {
            selected_page = 0;
            last_encoder_pos = 0;
            encoder.setPosition(0);
            last_hashrate = "";  // Force refresh
        }
    }
    
    if (digitalRead(ENC_SW) == HIGH && button_pressed) {
        button_pressed = false;
    }
}

#endif  // DISPLAY_TFT_H
