/*************************************************

Copyright © 2025 Jan K. S.
MIT License
https://github.com/wallysalami/QRCodeGFX

This example simulates a music player screen.
It has a QR Code for the video clip link.
It combines the QR Code with other drawing methods from Adafruit_GFX.

This program was tested on a ESP32 S3 with a WeAct 2.9'' Black-White E-Paper Display.
It should work with Waveshare E-Paper Display as well.
That display uses the SPI protocol to receive drawing commands.
The SPI pins changes from microcontroller to microcontroller.
Here are some examples:

Microcontroller | SDA (MOSI) pin | SCL (SCK) pin  
--------------------------------------------------
Arduino Uno     |       12       |       13       
Arduino Mega    |       51       |       52       
ESP32           |       23       |       18       
ESP32 S3        |       11       |       12       

You must also install the GxEPD2 library to compile it!

*************************************************/

#include <GxEPD2_BW.h>
#include <QRCodeGFX.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

// Adjust the pin numbers according to your wiring
// These were used with an ESP32 S3
#define CS_PIN 10
#define DC_PIN 14
#define RESET_PIN 15
#define BUSY_PIN 16

GxEPD2_290_T94_V2 configuration(CS_PIN, DC_PIN, RESET_PIN, BUSY_PIN);
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(configuration);

QRCodeGFX qrcode(display);

unsigned long previousInstant;
float x = 10;
float currentTime = 0;
float totalTime = (6*60 + 23); // in seconds

void setup() {
  // Initialize the display
  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(1);

  // Write music title, artist and album
  display.setFont(&FreeSansBold12pt7b);
  display.setCursor(10, 27);
  display.print("Black or White");
  display.setFont(&FreeSans9pt7b);
  display.setCursor(10, 62);
  display.print("Michael Jackson");
  display.setCursor(10, 82);
  display.print("Dangerous - 1991");

  // Draw QR Code for video link
  // The face transitions at the end are still incredible to watch.
  qrcode.setScale(3);
  qrcode.draw("https://youtu.be/F2AitTPI5U0", 202, 0);

  // Time slider
  updateTimeSlider();

  // Update display
  display.display(true);

  // Start counting song time
  previousInstant = millis();
}

void loop() {
  if (millis() > previousInstant + 1000 && currentTime < totalTime) {
    // some e-paper displays might take more than 1 second to update
    // WeAct displays take about 700ms
    currentTime += (millis() - previousInstant) / 1000;
    previousInstant = millis();

    updateTimeSlider();
  }
}

void formatTime(int time, char* string) {
  int minutes = time / 60;
  int seconds = time % 60;

  sprintf(string, "%02d:%02d", minutes, seconds);
}

void updateTimeSlider() {
  // Erase the bottom part of the display
  display.fillRect(0, 90, 300, 50, GxEPD_WHITE);

  // Update instants
  static char timeString[10];
  display.setFont(NULL);

  display.setCursor(10, 113);
  formatTime(currentTime, timeString);
  display.print(timeString);

  display.setCursor(257, 113);
  formatTime(totalTime, timeString);
  display.print(timeString);

  // Update slider
  x = 10 + 276.0f * currentTime / totalTime;
  display.drawRect(10, 100, 276, 6, GxEPD_BLACK);
  display.fillRect(10, 100, x-10, 6, GxEPD_BLACK);
  display.fillCircle(x, 102, 6, GxEPD_BLACK);
  
  // Refresh display
  display.display(true);
}
