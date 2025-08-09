
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display settings
#define MAX_OLED_PRINTLN_LENGTH 36
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// SD card settings
#define SD_CARD_SELECT_PIN 4
#define TIME_SYNC_LOG_FILE "time_sync_log.txt"
#define KEYS_FILE "keys.txt"
Sd2Card card;

void fatalError(const String &message);

void initDisplay();
void printTextToDisplay(const String &text, const int fontSize);

void initSD();
bool anyExpectedFilesExist();
bool allExpectedFilesExist();
void resetSDCard();
// void createNewFile(const char* fileName);

void setup() {
  // Initialize the USB serial communication
  Serial.begin(9600);

  // Initialize the display with a splash screen
  initDisplay();

  // Check that an SD card is present
  initSD();
}

void loop() {
}

/*
 *********************
 * Utility Functions *
 *********************
*/

void fatalError(const String &message) {
  // String errorMessage = String(F("Fatal: ")) + String(message);
  // errorMessage = errorMessage.substring(0, MAX_OLED_PRINTLN_LENGTH);
  Serial.println(F("FATAL_ERROR_FATAL_ERROR"));
  printTextToDisplay(String(F("FATAL: ")) + message, 1);
  for(;;); // Loop indefinitely to halt execution
}

/*
 **************************
 * OLED Display Functions *
 **************************
*/

void initDisplay() {
  Serial.println(F("Initializing display..."));
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
  Serial.println(text);
  delay(2000);
}

/*
 *********************
 * SD Card Functions *
 *********************
*/

void initSD() {
  Serial.println(F("Initializing SD card..."));

  // Wait for card to be plugged in
  if (!card.init(SPI_FULL_SPEED, SD_CARD_SELECT_PIN)) {
    printTextToDisplay(F("No SD card detected. Please Insert!"), 1);
    while (!card.init(SPI_FULL_SPEED, SD_CARD_SELECT_PIN)) {
      delay(1000);
    }
  }

  printTextToDisplay(F("SD card detected!"), 1);

  // Check for expected SD card files
  if (!allExpectedFilesExist()) {
    // TODO: Unenroll fingerprints
    // If any of the expected files are missing, reset the SD card
    printTextToDisplay(F("SD card is in an unknown state!"), 1);
    resetSDCard();
  } else {
    printTextToDisplay(F("All expected files exist."), 1);
  }
}

bool anyExpectedFilesExist() {
  return SD.exists(TIME_SYNC_LOG_FILE) || SD.exists(KEYS_FILE);
}

bool allExpectedFilesExist() {
  printTextToDisplay(F("Checking filesystem..."), 1);
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
    fatalError(F("Failed to create file!"));
  }
}

/**
 * Reset the files on the SD card.
 */
void resetSDCard() {
  printTextToDisplay(F("Resetting SD card..."), 1);

  // Remove all files on the SD card
  // Serial.println("Removing all files from SD card...");

  // If the formatting failed, error out
  // if (anyExpectedFilesExist()) {
  //   fatalError("SD card is corrupt and insecure.");
  // }

  createNewFile(TIME_SYNC_LOG_FILE);
  createNewFile(KEYS_FILE);

  if (!allExpectedFilesExist()) {
    fatalError(F("SD card is corrupt and insecure."));
  }

  // Ask the user for the keys
}