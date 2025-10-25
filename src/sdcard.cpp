#include "sdcard.h"
#include <SD.h>

// SD card settings
#define SD_CARD_SELECT_PIN 4
#define TIME_SYNC_LOG_FILE "time_sync_log.txt"
#define KEYS_FILE "keys.txt"
Sd2Card card;

void initSD() {
  // Wait for card to be plugged in
  if (!card.init(SPI_FULL_SPEED, SD_CARD_SELECT_PIN)) {
    while (!card.init(SPI_FULL_SPEED, SD_CARD_SELECT_PIN)) {
      delay(1000);
    }
  }

  // Check for expected SD card files
  if (!allExpectedFilesExist()) {
    resetSDCard();
  }
}

bool anyExpectedFilesExist() {
  return SD.exists(TIME_SYNC_LOG_FILE) || SD.exists(KEYS_FILE);
}

bool allExpectedFilesExist() {
  return SD.exists(TIME_SYNC_LOG_FILE) && SD.exists(KEYS_FILE);
}

void createNewFile(const char* fileName) {
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    file.close();
    Serial.print(F("Created file: "));
    Serial.println(fileName);
  } else {
    Serial.print(F("Failed to create file: "));
    Serial.println(fileName);
  }
}

/**
 * Reset the files on the SD card.
 */
void resetSDCard() {

  // Remove all files on the SD card
  // Serial.println("Removing all files from SD card...");

  // If the formatting failed, error out
  // if (anyExpectedFilesExist()) {
  //   fatalError("SD card is corrupt and insecure.");
  // }

  createNewFile(TIME_SYNC_LOG_FILE);
  createNewFile(KEYS_FILE);

  // Ask the user for the keys
}