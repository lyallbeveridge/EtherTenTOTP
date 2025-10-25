#include "oleddisplay.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MAX_OLED_PRINTLN_LENGTH 36
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("ETOTP!"));
  display.display();
  delay(2000);
}

void printTextToDisplay(const String &text, const int fontSize) {
  display.clearDisplay();
  display.setTextSize(fontSize);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
  delay(2000);
}