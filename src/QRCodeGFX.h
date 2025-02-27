/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

*************************************************/

#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "QRCodeGenerator.h"

enum class QRCodeRotation {
    R0 = 0,    // 0 degrees
    R90 = 1,   // 90 degrees
    R180 = 2,  // 180 degrees
    R270 = 3   // 270 degrees
};

class QRCodeGFX {
private:
  Adafruit_GFX& display;
  uint16_t scale = 1;
  QRCodeRotation rotation = QRCodeRotation::R0;
  uint16_t backgroundColor = 0xFFFF;
  uint16_t foregroundColor = 0x0000;
  QRCodeGenerator generator;
  uint8_t *qrcodeBuffer = NULL;

public:
  // Constructor
  QRCodeGFX(Adafruit_GFX& d);

  // Destructor
  ~QRCodeGFX();

  // Getters and setters
  QRCodeGFX& setScale(uint16_t s);
  uint16_t getScale();

  QRCodeGFX& setRotation(QRCodeRotation r);
  QRCodeRotation getRotation();

  QRCodeGFX& setColors(uint16_t backgroundColor, uint16_t foregroundColor);
  QRCodeGFX& setBackgroundColor(uint16_t color);
  uint16_t getBackgroundColor();
  QRCodeGFX& setForegroundColor(uint16_t color);
  uint16_t getForegroundColor();

  QRCodeGenerator& getGenerator();

  // Drawing methods that handle everything
  bool draw(const String &text, int16_t x, int16_t y);
  bool draw(const char *text, int16_t x, int16_t y);
  
  // You can also use these methods to generate the QRCode data and draw it later
  bool generateData(const String &text);
  bool generateData(const char *text);
  void releaseData();

  // Side length is determined by the QR Code version and the scale property
  // This method is usefull if you need to automatically center the drawing on screen
  // Call it after generating the data
  // Check the README for a full example
  int16_t getSideLength();

  // Draw the QRCode on the screen after generating the data
  // You can set releaseData to false if you want to draw the same QRCode multiple times
  bool draw(int16_t x, int16_t y, bool releaseData = true);
};