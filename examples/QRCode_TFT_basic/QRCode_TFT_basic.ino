/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

This example will draw a big QR Code on the screen.
It shows the basic usage of the library.

This program was tested on a Arduino Uno / Mega with a TFT Shield.

You must also install the MCUFRIEND_kbv library to compile it!

*************************************************/

#include <QRCodeGFX.h>
#include <MCUFRIEND_kbv.h>

// Initialize your display (example with LCD TFT Shield)
MCUFRIEND_kbv display;

// Create QR code drawer instance
QRCodeGFX qrcode(display);

void setup() {
  // Initialize your display first
  display.begin(display.readID());
  display.fillScreen(TFT_BLACK);

  // Prepare drawing parameters for a bigger size
  qrcode.setScale(6);
  
  // Draw the QR code on x = 27, y = 67
  qrcode.draw("https://youtu.be/dQw4w9WgXcQ", 27, 67);
}

void loop() {
}