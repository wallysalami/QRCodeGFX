/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

This example will keep drawing a rotating QR Code on the screen.
It shows how you can generate the QR Code data once and draw it many times.

This program was tested on a LilyGo TTGO Meshtastic 1.2 ESP32 LoRa.
It has a embedded SSD1306 0.96’’ monochrome OLED display.
Adjust the pin numbers if you are using a different device / wiring.

You must also install the Adafruit_SSD1306 library to compile it!

*************************************************/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <QRCodeGFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 21
#define SCL_PIN 22
#define RESET_PIN -1
#define I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN);

QRCodeGFX qrcode(display);

#define ROTATION_INTERVAL 3000 // miliseconds
unsigned long previousRotationInstant;
QRCodeRotation rotationAngle = QRCodeRotation::R0;

void setup() {
  Serial.begin(9600); delay(1000);

  // Initialize the display
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) { 
    Serial.println("SSD1306 allocation failed");
    while (true) {} // Don't proceed, loop forever
  }

  display.clearDisplay();

  // setBackgroundColor is important here
  // WHITE = 0x01, default = 0xFF
  // passing color value 0xFF to Adafruit_SSD1306 causes weird bugs sometimes
  qrcode.setBackgroundColor(WHITE).setScale(2);

  // Generate QR Code data
  // It's an old but awesome Brazilian song. =)
  qrcode.generateData("https://youtu.be/0dU4mLOHGIQ");
  
  // Draw generated QR Code and don't release its data from memory
  // This way we can keep drawing it again and again without having to generate it
  qrcode.draw(0, 0, false);

  // Write song title and artist
  display.setTextColor(WHITE);
  display.setCursor(70, 10);
  display.print("RODA VIVA");
  display.setCursor(70, 30);
  display.print("Chico");
  display.setCursor(70, 40);
  display.print("Buarque");
  display.setCursor(70, 50);
  display.print("e MPB4");

  // Update the display
  display.display();
}

void loop() {
  // Every 3s, rotate the QR Code 90˚ and draw it again on screen
  if (millis() > previousRotationInstant + ROTATION_INTERVAL) {
    previousRotationInstant = millis();

    // You could also write something like:
    //   rotationAngle = (QRCodeRotation) (((int)rotationAngle + 1) % 4);
    // But I think the ifs are clearer
    if (rotationAngle == QRCodeRotation::R0) {
      rotationAngle = QRCodeRotation::R90;
    }
    else if (rotationAngle == QRCodeRotation::R90) {
      rotationAngle = QRCodeRotation::R180;
    }
    else if (rotationAngle == QRCodeRotation::R180) {
      rotationAngle = QRCodeRotation::R270;
    }
    else {
      rotationAngle = QRCodeRotation::R0;
    }
    qrcode.setRotation(rotationAngle);
    
    qrcode.draw(0, 0, false);

    display.display();
  }
}