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

  int16_t getSideLength();

public:
  QRCodeGFX(Adafruit_GFX& d);

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

  bool draw(const String &text, int16_t x, int16_t y);
  bool draw(const char *text, int16_t x, int16_t y);

  // Side length is determined by the text size, the scale property and error correction level
  // This method is usefull if you need to automatically center the drawing on screen
  // Check the README for a full example more details
  int16_t generate(const String &text);
  int16_t generate(const char *text);

  bool draw(int16_t x, int16_t y, bool releaseAllocatedMemory = true);
};