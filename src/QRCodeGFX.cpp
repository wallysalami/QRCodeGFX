/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

*************************************************/

#include "QRCodeGFX.h"

// Constructor

QRCodeGFX::QRCodeGFX(Adafruit_GFX& d): display(d) {
}

// Destructor

QRCodeGFX::~QRCodeGFX() {
  releaseData();
}

// Getters and setters

QRCodeGFX& QRCodeGFX::setScale(uint16_t _scale) {
  static constexpr uint16_t MAX_SAFE_SCALE = 100;
    
  if (_scale == 0) {
    scale = 1;
  } else if (_scale > MAX_SAFE_SCALE) {
    scale = MAX_SAFE_SCALE;
  } else {
    scale = _scale;
  }

  return *this;
}

uint16_t QRCodeGFX::getScale() {
  return scale;
}

QRCodeGFX& QRCodeGFX::setRotation(QRCodeRotation r) {
  rotation = r;

  return *this;
}

QRCodeRotation QRCodeGFX::getRotation() {
  return rotation;
}

QRCodeGFX& QRCodeGFX::setColors(uint16_t _backgroundColor, uint16_t _foregroundColor) {
  backgroundColor = _backgroundColor;
  foregroundColor = _foregroundColor;

  return *this;
}

QRCodeGFX& QRCodeGFX::setBackgroundColor(uint16_t color) {
  backgroundColor = color;

  return *this;
}

uint16_t QRCodeGFX::getBackgroundColor() {
  return backgroundColor;
}

QRCodeGFX& QRCodeGFX::setForegroundColor(uint16_t color) {
  foregroundColor = color;

  return *this;
}

uint16_t QRCodeGFX::getForegroundColor() {
  return foregroundColor;
}

QRCodeGenerator& QRCodeGFX::getGenerator() {
  return generator;
}

// Drawing methods

bool QRCodeGFX::draw(const String &text, int16_t x, int16_t y) {
  return draw(text.c_str(), x, y);
}

bool QRCodeGFX::draw(const char* text, int16_t x, int16_t y) {
  bool success = generateData(text);
  if (success == false) {
    return false;
  }

  return draw(x, y, true);
}

bool QRCodeGFX::draw(int16_t x, int16_t y, bool releaseQRCodeData) {
  int16_t x0 = x, y0 = y;
  int size = getSideLength();
  if (size == 0) {
    return false; // error: qrcode not generated before calling draw
  }
  display.fillRect(x0, y0, size, size, backgroundColor);

  int padding = scale * 3;

  int qrcodeSize = qrcodegen_getSize(qrcodeBuffer);
  for (uint8_t i = 0; i < qrcodeSize; i++) {
    for (uint8_t j = 0; j < qrcodeSize; j++) {
      int x, y;
      switch(rotation) {
        case QRCodeRotation::R0:
          x = x0 + padding + scale * j;
          y = y0 + padding + scale * i;
          break;
        case QRCodeRotation::R90:
          x = x0 + padding + scale * (qrcodeSize - i - 1);
          y = y0 + padding + scale * j;
          break;
        case QRCodeRotation::R180:
          x = x0 + padding + scale * (qrcodeSize - j - 1);
          y = y0 + padding + scale * (qrcodeSize - i - 1);
          break;
        case QRCodeRotation::R270:
          x = x0 + padding + scale * i;
          y = y0 + padding + scale * (qrcodeSize - j - 1);
          break;
      }
      uint16_t color = qrcodegen_getModule(qrcodeBuffer, j, i) ? foregroundColor : backgroundColor;
      if (scale == 1) {
        display.drawPixel(x, y, color);
      }
      else {
        display.fillRect(x, y, scale, scale, color);
      }
    }
  }

  if (releaseQRCodeData) {
    releaseData();
  }

  return true;
}

// Generation methods

bool QRCodeGFX::generateData(const String &text) {
  return generateData(text.c_str());
}

bool QRCodeGFX::generateData(const char *text) {
  releaseData();

  qrcodeBuffer = generator.generateData(text);

  return qrcodeBuffer != NULL;
}

int16_t QRCodeGFX::getSideLength() {
  if (qrcodeBuffer == NULL) {
    return 0;
  }
  uint8_t size = qrcodegen_getSize(qrcodeBuffer);
  int padding = scale * 3;
  return size * scale + 2 * padding;
}

void QRCodeGFX::releaseData() {
  if (generator.getQRCodeBuffer() == NULL && qrcodeBuffer != NULL) {
    free(qrcodeBuffer);
  }

  qrcodeBuffer = NULL;
}