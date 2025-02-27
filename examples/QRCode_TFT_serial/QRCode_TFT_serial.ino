/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

This example will ask the user to input some text on Serial Monitor.
Then it will draw a QR Code for that text at the center of the screen.
It will calculate and use the maximum QR Code size that fits the screen.
It's an interesting example of QR Code generation from an arbitrary text.

This program was tested on a Arduino Uno / Mega with a TFT Shield.

You must also install the MCUFRIEND_kbv library to compile it!

*************************************************/


#include <MCUFRIEND_kbv.h>
#include <QRCodeGFX.h>

MCUFRIEND_kbv display;

QRCodeGFX qrcode(display);

void setup() {
  Serial.begin(9600);

  // Initialize the display
  display.begin(display.readID());
  display.fillScreen(TFT_BLACK);
  
  // Write instructions on the screen
  display.setTextSize(2);
  display.setTextColor(TFT_WHITE);
  display.setCursor(12, 130);
  display.print("Enter some text in");
  display.setCursor(18, 160);
  display.print("Serial Monitor...");
}

void loop() {
  // Draw a big QR Code if user sends a text via Serial
  if (Serial.available()) {
    // Get text sent by user
    String text = Serial.readStringUntil('\n');
    Serial.println(text);

    // Erase previous QR Code
    display.fillScreen(TFT_BLACK);

    // Find the biggest scale that fits the screen
    int version = qrcode.getGenerator().getMinVersion(text);
    int baseSideLength = (version * 4 + 17 + 6);
    int scale = display.width() / baseSideLength;
    int finalSideLength = baseSideLength * scale;
    
    qrcode.setScale(scale);

    // Center the qr code
    int x = (display.width() - finalSideLength) / 2;
    int y = (display.height() - finalSideLength) / 2;

    // Draw it!
    bool success = qrcode.draw(text, x, y);
    if (!success) {
      display.setTextSize(2);
      display.setTextColor(TFT_WHITE);
      display.setCursor(37, 150);
      display.print("TEXT TOO LONG!");
    }
  }
}