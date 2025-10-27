/* Written by davenport651 @ 10/14/25 with help from AI tools
Should compile for ESP8266mod and display on a generic 0.96" OLED over I2C

Wiring diagram for OLED -> ESP8266
VCC → 3.3V
GND → GND
SCL → D1 (GPIO5)
SDA → D2 (GPIO4)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tornado_animation.h"  // Contains tornado_frames[] and tornado_frame_count

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
//const char currency_symbols[] = {'$', '¢', '£', '¥', '€'};  //requires extended ASCII library 
//const char currency_symbols[] = {'$', '#', '%', '&', '@'};  //more symbols, if you prefer
const char currency_symbols[] = {'$'};
const int num_symbols = sizeof(currency_symbols);

// Frame timing
unsigned long lastFrameTime = 0;
const int frameDelay = 300;  // SPEED IN ms
int currentFrame = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(0));
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
    for (int i=0; i <random(1, 2); i++) {
      int x = random(0, SCREEN_WIDTH - 8);
      int y = random(0, SCREEN_HEIGHT - 8);
      display.drawBitmap(x,y, dlogo, 30, 24, SSD1306_WHITE);
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
