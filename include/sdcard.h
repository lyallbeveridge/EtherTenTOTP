#ifndef SDCARD_H
#define SDCARD_H

void initSD();
bool anyExpectedFilesExist();
bool allExpectedFilesExist();
void createNewFile();
void resetSDCard();

#endif