#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Arduino.h>

void initDisplay();
void printTextToDisplay(const String &text, const int fontSize);

#endif