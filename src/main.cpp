
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display settings
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

void printTextToDisplay(const String &text, const int fontSize);
bool allExpectedFilesExist();

void fatalError(const String &message) {
  printTextToDisplay("Fatal Error: " + message, 2);
  for(;;); // Loop indefinitely to halt execution
}

void initDisplay() {
  Serial.println("Initializing display...");
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

// bool anyExpectedFilesExist() {
//   return SD.exists(TIME_SYNC_LOG_FILE) || SD.exists(KEYS_FILE);
// }

// void createNewFile(const String &fileName) {
//   File file = SD.open(fileName, FILE_WRITE);
//   if (file) {
//     file.close();
//     Serial.println("Created file: " + fileName);
//   } else {
//     Serial.println("Failed to create file: " + fileName);
//     fatalError("Failed to create " + fileName);
//   }
// }

// /**
//  * Reset the files on the SD card.
//  */
// void resetSDCard() {
//   printTextToDisplay("Resetting SD card...", 1);

//   // Remove all files on the SD card
//   Serial.println("Removing all files from SD card...");
//   SD.rmdir("/");

//   // If the formatting failed, error out
//   if (anyExpectedFilesExist()) {
//     fatalError("SD card is corrupt and insecure.");
//   }

//   createNewFile(TIME_SYNC_LOG_FILE);
//   createNewFile(KEYS_FILE);

//   if (!allExpectedFilesExist()) {
//     fatalError("SD card is corrupt and insecure.");
//   }

//   // Ask the user for the keys
// }

void initSD() {
  Serial.println("Initializing SD card...");

  // Wait for card to be plugged in
  if (!card.init(SPI_HALF_SPEED, SD_CARD_SELECT_PIN)) {
    printTextToDisplay("No SD card detected. Please Insert!", 1);
    while (!card.init(SPI_HALF_SPEED, SD_CARD_SELECT_PIN)) {
      delay(1000);
    }
  }

  printTextToDisplay("SD card detected!", 1);

  // Check for expected SD card files
  if (!allExpectedFilesExist()) {
    // TODO: Unenroll fingerprints
    // If any of the expected files are missing, reset the SD card
    // resetSDCard();

    printTextToDisplay("TODO: reset card!", 1);
  }
}

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

void printTextToDisplay(const String &text, const int fontSize) {

  display.clearDisplay();
  display.setTextSize(fontSize);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
  delay(2000);
  Serial.println("Display: " + text);
}

/*
 * SD Card Functions
 */

bool allExpectedFilesExist() {
  printTextToDisplay("Checking filesystem...", 1);
  return SD.exists(TIME_SYNC_LOG_FILE) && SD.exists(KEYS_FILE);
}