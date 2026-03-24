// File: User_Setup.h (đặt trong thư mục dự án hoặc cấu hình TFT_eSPI)
// CẤU HÌNH CHO MÀN HÌNH ST7789 2.0 INCH
// ĐÃ SỬA CHÂN THEO YÊU CẦU: CS=13, các chân khác giữ nguyên

#define ST7789_DRIVER

// ==================== CHÂN KẾT NỐI TFT ====================
// ĐÃ SỬA TFT_CS TỪ 8 -> 13
#define TFT_CS   13   // Chip Select - GPIO13 (đã sửa)
#define TFT_DC   9    // Data/Command - GPIO9
#define TFT_RST  10   // Reset - GPIO10
#define TFT_SCLK 12   // SPI Clock - GPIO12
#define TFT_MOSI 11   // SPI Data - GPIO11

// Nếu dùng MISO (không bắt buộc với màn hình chỉ ghi)
// #define TFT_MISO 13

// ==================== KÍCH THƯỚC MÀN HÌNH ====================
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ==================== TẦN SỐ SPI ====================
#define SPI_FREQUENCY  40000000   // 40 MHz
#define SPI_READ_FREQUENCY  20000000  // 20 MHz

// ==================== TÙY CHỌN THÊM ====================
// Đảo chiều màu (nếu cần)
// #define TFT_INVERSION_ON

// Chân backlight (sẽ được điều khiển riêng trong code)
// #define TFT_BL   46   // Không cần định nghĩa ở đây vì đã có trong DisplayTFT.h

// ==================== GHI CHÚ ====================
// Nếu dùng nhiều màn hình, có thể bật:
// #define SUPPORT_TRANSACTIONS
