#include <MCUFRIEND_kbv.h>
#include <QRCodeGFX.h>

MCUFRIEND_kbv display;

QRCodeGFX qrcode(display);

char text[] = "https://youtu.be/iW_Ct49H1ng";

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
  {130,  90, 3, QRCodeRotation::R90,  QRCodeECCLevel::Medium,   TFT_YELLOW, TFT_BLUE},
  {10,  170, 3, QRCodeRotation::R180, QRCodeECCLevel::Quartile, TFT_GREEN,  TFT_PURPLE},
  {140, 230, 2, QRCodeRotation::R270, QRCodeECCLevel::High,     TFT_PINK,   TFT_MAROON},
};

void setup() {
  Serial.begin(9600);

  // Initialize the display
  display.begin(display.readID());
  display.fillScreen(TFT_BLACK);
  display.setRotation(0); // portrait orientation

  // Calculte the positions
  int numberOfSamples = sizeof(samples) / sizeof(QRCodeSample);

  // Draw sample barcodes
  for (int i = 0; i < numberOfSamples; i++) {

    qrcode.setScale(samples[i].scale)
          .setRotation(samples[i].rotation)
          .setColors(samples[i].backgroundColor, samples[i].foregroundColor);

    qrcode.getGenerator().setErrorCorrectionLevel(samples[i].level);

    qrcode.draw(text, samples[i].x, samples[i].y);
  }
}

void loop() {
}
