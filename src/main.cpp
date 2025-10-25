
#include <SPI.h>
#include "rtc.h"
#include "sdcard.h"
#include "oleddisplay.h"


void setup() {
  // Initialize the USB serial communication
  Serial.begin(9600);

  // Initialize the display with a splash screen
  Serial.println(F("Initialising display..."));
  initDisplay();

  // Check that an SD card is present
  // initSD();

  // Initialize the RTC
  delay(2000);
  Serial.println(F("Initialising RTC Module"));
  initRTC();
}

void loop() {
  Serial.println((unsigned long)getUnixTime());
  delay(1000);
}