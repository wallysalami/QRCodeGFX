#include <Arduino.h>
#include "qrcodegen.h"

// QRCode minimum version detection uses static tables
// That might be a problem for some devices with low memory (e.g. Arduino Uno)
// You can save memory by defining QRCODE_GENERATOR_USE_LESS_MEMORY here (uncomment the line below)
// This will disable the tables and some methods at compile time, reducing memory usage
// In that case, you must always manually set the version to generate the QRCode
// Please check the README to see the text size limits for each version
// Then you can use the macro qrcodegen_BUFFER_LEN_FOR_VERSION(n)

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
  uint8_t *qrcodeBuffer = NULL; // in case you want to use static memory
  uint8_t *tempBuffer = NULL;   // in case you want to use static memory
  uint16_t bufferSize = 0;

public:
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
  uint8_t getMinVersion(const String &text);
  uint8_t getMinVersion(const char* text);

  uint16_t getBufferSize(const String &text);
  uint16_t getBufferSize(const char* text);
#endif

  uint8_t* generateQRCode(const String &text);
  uint8_t* generateQRCode(const char* text);
};