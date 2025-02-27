/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

*************************************************/

#include "QRCodeGenerator.h"

#ifndef QRCODE_GENERATOR_USE_LESS_MEMORY
#include "QRCodeLimits.h"
#endif

// Getters and setters

QRCodeGenerator& QRCodeGenerator::setErrorCorrectionLevel(QRCodeECCLevel level) {
  errorCorrectionLevel = level;
  return *this;
}

QRCodeECCLevel QRCodeGenerator::getErrorCorrectionLevel() {
  return errorCorrectionLevel;
}

QRCodeGenerator& QRCodeGenerator::setBuffers(uint8_t *qrcodeBuffer, uint8_t* tempBuffer, uint16_t size) {
  this->qrcodeBuffer = qrcodeBuffer;
  this->tempBuffer = tempBuffer;
  this->bufferSize = size;
  return *this;
}

uint8_t* QRCodeGenerator::getQRCodeBuffer() {
  return qrcodeBuffer;
}

uint8_t* QRCodeGenerator::getTempBuffer() {
  return tempBuffer;
}

uint16_t QRCodeGenerator::getBufferSize() {
  return bufferSize;
}

QRCodeGenerator& QRCodeGenerator::setVersion(uint8_t version) {
  return setVersionRange(version, version);
}

QRCodeGenerator& QRCodeGenerator::setVersionRange(uint8_t minVersion, uint8_t maxVersion) {
  if (minVersion > maxVersion) {
    uint8_t temp = minVersion;
    minVersion = maxVersion;
    maxVersion = temp;
  }

  if (minVersion < 0) {
    minVersion = 1;
  }
  if (maxVersion > 40) {
    maxVersion = 40;
  }
  this->minVersion = minVersion;
  this->maxVersion = maxVersion;
  return *this;
}

uint8_t QRCodeGenerator::getMinVersion() {
  return minVersion;
}

uint8_t QRCodeGenerator::getMaxVersion() {
  return maxVersion;
}

#ifndef QRCODE_GENERATOR_USE_LESS_MEMORY

// Automatic version detection and buffer size calculation methods
// Disabled if QRCODE_GENERATOR_USE_LESS_MEMORY is defined

uint8_t QRCodeGenerator::getMinVersion(const String &text) {
  return getMinVersion(text.c_str());
}

uint8_t QRCodeGenerator::getMinVersion(const char* text) {
  int length = strlen(text);
  int charsPerVersion;

  int errorMode = (int)errorCorrectionLevel;

  uint16_t capacity = QR_BYTE_BASE[errorMode];
  bool add100 = false;
  const uint8_t (*diff)[4] = QR_BYTE_DIFF;
  if (qrcodegen_isNumeric(text)) {
    capacity = QR_NUMERIC_BASE[errorMode];
    diff = QR_NUMERIC_DIFF;
    add100 = true;
  } else if (qrcodegen_isAlphanumeric(text)) {
    capacity = QR_ALPHANUMERIC_BASE[errorMode];
    diff = QR_ALPHANUMERIC_DIFF;
  }
  
  if (length <= capacity) {
    return 1;
  }
  
  // Find the minimum version needed
  for (int version = 2; version <= 40; version++) {
    capacity += diff[version-2][errorMode];
    // as mentioned in the qrcodegen.h file, I had to subtract 100 from some values to keep them as 8-bit integers
    if (add100 && version > 30 && errorMode < 2) {
      capacity += 100;
    }
    if (length <= capacity) {
      return version;
    }
  }

  return 0; // Invalid version
}

uint16_t QRCodeGenerator::getBufferSize(const String &text) {
  return getBufferSize(text.c_str());
}

uint16_t QRCodeGenerator::getBufferSize(const char* text) {
  int length = strlen(text);
  int version = getMinVersion(text);
  if (version == 0) {
    return 0;
  }
  return qrcodegen_BUFFER_LEN_FOR_VERSION(version);
}

#endif

// Generation methods

uint8_t* QRCodeGenerator::generateData(const String &text) {
  return generateData(text.c_str());
}

uint8_t* QRCodeGenerator::generateData(const char* text) {
  uint8_t minVersion = getMinVersion();
  uint8_t maxVersion = getMaxVersion();

#ifndef QRCODE_GENERATOR_USE_LESS_MEMORY
  uint8_t minVersionForText = getMinVersion(text);
  if (minVersionForText > maxVersion) {
    return NULL;
  }

  uint8_t version = minVersionForText;
  if (minVersionForText < minVersion) {
    version = minVersion;
  }

  minVersion = version;
  maxVersion = version;
#else
  uint8_t version = maxVersion;
#endif

  uint8_t *qrcodeBuffer = this->qrcodeBuffer;
  uint8_t *tempBuffer = this->tempBuffer;
  uint16_t bufferSizeForVersion = qrcodegen_BUFFER_LEN_FOR_VERSION(version);

  if (qrcodeBuffer == NULL) { // no buffer provided
    qrcodeBuffer = (uint8_t *)malloc(bufferSizeForVersion);
    if (qrcodeBuffer == NULL) {
      return NULL; // error: not enough memory
    }
  }
  else if (bufferSizeForVersion > bufferSize) {
    return NULL;  // error: provided buffer too small for required version
  }

  if (tempBuffer == NULL) { // no buffer provided
    tempBuffer = (uint8_t *)malloc(bufferSizeForVersion);
    if (tempBuffer == NULL) {
      if (this->qrcodeBuffer == NULL) {
        free(qrcodeBuffer);
      }
      return NULL; // error: not enough memory
    }
  }
  else if (bufferSizeForVersion > bufferSize) {
    if (this->qrcodeBuffer == NULL) {
      free(qrcodeBuffer);
    }
    return NULL;  // error: provided buffer too small for required version
  }

  bool success = qrcodegen_encodeText(text, tempBuffer, qrcodeBuffer, (enum qrcodegen_Ecc)errorCorrectionLevel,
                                      minVersion, maxVersion, qrcodegen_Mask_AUTO, false);

  if (this->tempBuffer == NULL) {
    free(tempBuffer);
  }

  if (!success) {
    if (this->qrcodeBuffer == NULL) {
      free(qrcodeBuffer);
    }
    return NULL;
  }

  return qrcodeBuffer;
}