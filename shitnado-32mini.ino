// GitHub Copilot Chat Assistant
// ESP32-C3 Super Mini adapted version of shitnado.ino
// - Set SDA_PIN and SCL_PIN to the pins you wired the OLED to.
// - Uses esp_random() for seeding (ESP32-safe)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tornado_animation.h"  // Contains tornado_frames[] and tornado_frame_count
#include <esp_system.h>        // for esp_random()

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Set these to the pins you used on your ESP32-C3 Super Mini.
// Example placeholders (change to match your wiring):
// For example, if you wired SDA to GPIO6 and SCL to GPIO7, set them accordingly.
// If you prefer to use the default I2C pins for your board, you can call Wire.begin(); instead.
#define SDA_PIN 3
#define SCL_PIN 4

// Poop sprite (8x8)
const unsigned char poop_sprite[] PROGMEM = {
  // 'poop, 8x8px
  0x02, 0x0c, 0x3c, 0x5a, 0x5a, 0xfe, 0x47, 0xff
};
static const unsigned char PROGMEM dlogo[] = {
 // 'd1b, 30x24px
  0x00, 0x0f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0xef, 0xe0, 0x00, 
  0x00, 0xfc, 0x70, 0x00, 0x01, 0xfd, 0xf8, 0x00, 0x01, 0x96, 0x7c, 0x00, 0x02, 0x7b, 0xfc, 0x00, 
  0x07, 0xef, 0xde, 0x00, 0x07, 0xdf, 0xde, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xeb, 0x00, 
  0x1f, 0xff, 0xfd, 0x80, 0x3f, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xfd, 0xc0, 0x7f, 0xff, 0xfb, 0xe0, 
  0x7f, 0xff, 0xfb, 0xe0, 0x7f, 0xfd, 0x77, 0xe0, 0x7f, 0x7c, 0x2f, 0xf0, 0xfe, 0xf8, 0x2f, 0xf0, 
  0xfe, 0xc0, 0x6f, 0xf0, 0xf8, 0x00, 0x7f, 0xf8, 0x70, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x0b, 0x40
};

// Currency symbols (ASCII)
const char currency_symbols[] = {'$'};
const int num_symbols = sizeof(currency_symbols);

// Frame timing
unsigned long lastFrameTime = 0;
const int frameDelay = 300;  // SPEED IN ms
int currentFrame = 0;

void setup() {
  // Initialize I2C for the OLED on the pins you selected
  Wire.begin(SDA_PIN, SCL_PIN);
  // If you prefer to use the default/built-in I2C pins for your board, use:
  // Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Failed to initialize display — you can add debugging here if desired
    for (;;) delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Seed random from hardware RNG on ESP32
  randomSeed((unsigned long)esp_random());
}

void loop() {
  unsigned long now = millis();
  if (now - lastFrameTime >= frameDelay) {
    lastFrameTime = now;

    display.clearDisplay();

    // Draw tornado frame
    display.drawBitmap(0, 0, tornado_frames[currentFrame], 128, 64, SSD1306_WHITE);

    // Draw random poop sprites
    for (int i = 0; i < random(2, 5); i++) {
      int x = random(0, SCREEN_WIDTH - 8);
      int y = random(0, SCREEN_HEIGHT - 8);
      display.drawBitmap(x, y, poop_sprite, 8, 8, SSD1306_WHITE);
    }

    // Draw random dlogo
    for (int i=0; i < random(1, 2); i++) {
      int x = random(0, SCREEN_WIDTH - 8);
      int y = random(0, SCREEN_HEIGHT - 8);
      display.drawBitmap(x, y, dlogo, 30, 24, SSD1306_WHITE);
    }

    // Draw random currency symbols
    for (int i = 0; i < random(3, 6); i++) {
      int x = random(0, SCREEN_WIDTH - 6);
      int y = random(0, SCREEN_HEIGHT - 8);
      char symbol = currency_symbols[random(0, num_symbols)];
      display.setCursor(x, y);
      display.write(symbol);
    }

    display.display();

    // Advance to next frame
    currentFrame = (currentFrame + 1) % tornado_frame_count;
  }
}
