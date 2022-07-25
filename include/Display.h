/*
 */

#ifndef DISPLAY_H
#define DISPLAY_H

void displaySetup();
void guiRun();
void mainScreen();
bool setDisplayBrightness(uint8_t value, void* cbData=nullptr);
void displayBootScreen();
size_t getTouchCalibrationJson(char* buffer, size_t bSize=1<<30);

// Switch from portrait to landscape
#define LCD_WIDTH 480
#define LCD_HEIGHT 320

#endif // DISPLAY_H