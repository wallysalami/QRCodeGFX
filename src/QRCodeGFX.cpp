#include "QRCodeGFX.h"

// Constructor

QRCodeGFX::QRCodeGFX(Adafruit_GFX& d): display(d) {
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
  qrcodeBuffer = generator.generateQRCode(text);
  if (qrcodeBuffer == NULL) {
    return false;
  }

  draw(x, y, true);

  return true;
}

bool QRCodeGFX::draw(int16_t x, int16_t y, bool releaseAllocatedMemory) {
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

  if (generator.getQRCodeBuffer() == NULL && releaseAllocatedMemory) {
    free(qrcodeBuffer);
  }

  return true;
}

// Generation methods

int16_t QRCodeGFX::generate(const String &text) {
  return generate(text.c_str());
}

int16_t QRCodeGFX::generate(const char *text) {
  qrcodeBuffer = generator.generateQRCode(text);

  return getSideLength();
}

// Private methods

int16_t QRCodeGFX::getSideLength() {
  if (qrcodeBuffer == NULL) {
    return 0;
  }
  uint8_t size = qrcodegen_getSize(qrcodeBuffer);
  int padding = scale * 3;
  return size * scale + 2 * padding;
}