/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

*************************************************/

#pragma once

#include <Arduino.h>
#include "qrcodegen.h"

// QRCode minimum version detection uses some big static variables as tables
// That might be a problem for some devices with low memory (e.g. Arduino Uno)
// You can save memory by defining QRCODE_GENERATOR_USE_LESS_MEMORY here (uncomment the line below)
// This will disable the tables and some methods at compile time, reducing memory usage
// In that case, you must set the version range according to your device capabilities
// Please check the text size limits for each version at the link below
// https://github.com/ricmoo/QRCode/tree/master?tab=readme-ov-file#data-capacities

//#define QRCODE_GENERATOR_USE_LESS_MEMORY

enum class QRCodeECCLevel {
  Low,
  Medium,
  Quartile,
  High
};

class QRCodeGenerator {
private:
  QRCodeECCLevel errorCorrectionLevel = QRCodeECCLevel::Low;
  uint8_t minVersion = 1;
  uint8_t maxVersion = 40;

  // In case you prefer to use static variables instead of dynamic memory allocation
  uint8_t *qrcodeBuffer = NULL; 
  uint8_t *tempBuffer = NULL;
  uint16_t bufferSize = 0;

public:
  // Getters and setters
  QRCodeGenerator& setErrorCorrectionLevel(QRCodeECCLevel level);
  QRCodeECCLevel getErrorCorrectionLevel();
  QRCodeGenerator& setVersion(uint8_t version);
  QRCodeGenerator& setVersionRange(uint8_t minVersion, uint8_t maxVersion);
  uint8_t getMinVersion();
  uint8_t getMaxVersion();
  QRCodeGenerator& setBuffers(uint8_t *qrcodeBuffer, uint8_t *tempBuffer, uint16_t size);
  uint8_t* getQRCodeBuffer();
  uint8_t* getTempBuffer();
  uint16_t getBufferSize();

#ifndef QRCODE_GENERATOR_USE_LESS_MEMORY
  // Methods to get the minimum version / buffer size required to generate the QRCode
  // They are suppressed when using less memory
  uint8_t getMinVersion(const String &text);
  uint8_t getMinVersion(const char* text);

  uint16_t getBufferSize(const String &text);
  uint16_t getBufferSize(const char* text);
#endif

  // Generation methods
  uint8_t* generateData(const String &text);
  uint8_t* generateData(const char* text);
};