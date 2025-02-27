/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

This example will draw different QR Codes from an array of configurations.
It shows many of the customization options of the library.

This program was tested on a Arduino Uno with a TFT Shield.

You must also install the MCUFRIEND_kbv library to compile it!

*************************************************/

#include <MCUFRIEND_kbv.h>
#include <QRCodeGFX.h>

MCUFRIEND_kbv display;

QRCodeGFX qrcode(display);

char text[] = "https://youtu.be/iW_Ct49H1ng";

// A good buffer size to run this program in Arduino Uno
#define BUFFER_SIZE 175
uint8_t qrcodeBuffer[BUFFER_SIZE];
uint8_t tempBuffer[BUFFER_SIZE];

struct QRCodeSample {
  int x;
  int y;
  uint8_t scale;
  QRCodeRotation rotation;
  QRCodeECCLevel level;
  uint16_t backgroundColor;
  uint16_t foregroundColor;
};

QRCodeSample samples[] = {
  {0,     0, 4, QRCodeRotation::R0,   QRCodeECCLevel::Low,      TFT_WHITE,  TFT_BLACK},
  {130,  50, 3, QRCodeRotation::R90,  QRCodeECCLevel::Medium,   TFT_YELLOW, TFT_BLUE},
  {10,  140, 3, QRCodeRotation::R180, QRCodeECCLevel::Quartile, TFT_GREEN,  TFT_PURPLE},
  {140, 180, 2, QRCodeRotation::R270, QRCodeECCLevel::High,     TFT_PINK,   TFT_MAROON},
};

void setup() {
  Serial.begin(9600);

  // Initialize the display
  display.begin(display.readID());
  display.fillScreen(TFT_BLACK);
  display.setRotation(0); // portrait orientation

  // Calculte the positions
  int numberOfSamples = sizeof(samples) / sizeof(QRCodeSample);

  // Use static buffers to avoid memory allocations (which may cause heap fragmentation)
  qrcode.getGenerator().setBuffers(qrcodeBuffer, tempBuffer, sizeof(qrcodeBuffer));

  // Draw sample barcodes
  for (int i = 0; i < numberOfSamples; i++) {

    qrcode.setScale(samples[i].scale)
          .setRotation(samples[i].rotation)
          .setColors(samples[i].backgroundColor, samples[i].foregroundColor);

    qrcode.getGenerator().setErrorCorrectionLevel(samples[i].level);

    qrcode.draw(text, samples[i].x, samples[i].y);
  }

  // Song title and artist
  display.setTextSize(2);
  display.setCursor(10, 265);
  display.print("Mega Man 2 - Dr.");
  display.setCursor(10, 282);
  display.print("Wily Stage Acapella");
  display.setCursor(10, 300);
  display.print("Smooth McGroove");
}

void loop() {
}
