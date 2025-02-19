# QRCodeGFX

QRCodeGFX is an Arduino library for drawing QR codes on displays. It is designed to be display-agnostic and works with any library that inherits from Adafruit_GFX.

![Image](https://github.com/user-attachments/assets/caadc842-7797-44c6-9f1d-c2ca64a4cd60)
*Example of QR Codes drawn on different displays*

## Features
- Works with any display compatible with Adafruit_GFX library:
  - Adafruit_SSD1306 (monochrome OLED)
  - Adafruit_ILI9341 (TFT LCD SPI)
  - MCUFRIEND_kbv (TFT LCD)
  - GxEPD2 (E-Paper)
  - And many others
- Smart QR code handling:
  - Automatic version detection (from 1 to 40)
  - Automatic encoding mode selection (numeric, alphanumeric, or byte)
  - Automatic memory management
- Customizable generation:
  - Adjustable version range (from custom min to custom max)
  - Adjustable error correction level
  - Static array buffers as optional parameters to avoid heap allocation
- Customizable appearance:
  - Adjustable scale (size)
  - Configurable colors
  - Customizable rotation (0°, 90°, 180°, 270°)

## Installation

1. In the Arduino IDE, go to Sketch -> Include Library -> Manage Libraries...
2. Search for QRCodeGFX
3. Click Install

Or if you want to do it manually:

1. Download the ZIP file of this repository
2. In the Arduino IDE, go to Sketch -> Include Library -> Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart the Arduino IDE

## Usage

### Basic Example
```cpp
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
  
  // Draw a small black and white QR code
  // Parameters:
  //   text: content to encode
  //   x: horizontal position (upper left corner)
  //   y: vertical position (upper left corner)
  qrcode.draw("https://youtu.be/dQw4w9WgXcQ", 10, 10);

  // Cameras might not detect a QR Code this small on a TFT display
  // Check next example to see how to increase the scale
}

void loop() {
}
```

### Customized Drawing
```cpp
// Configure appearance using method chaining
qrcode.setScale(3)                       // from 1 to 20
      .setColors(TFT_YELLOW, TFT_BLUE)   // background, foreground
      .setRotation(QRCodeRotation::R90); // 0°, 90°, 180°, or 270°

// Configure qrcode generation
qrcode.getGenerator()
      .setErrorCorrectionLevel(QRCodeECCLevel::High)
      .setVersion(5); // force specific version

// Draw and check for errors
String text = "https://youtu.be/O0_H3F84Yjk";

bool success = qrcode.draw(text, 0, 0);
if (!success) {
  // Error generating QR code!
  // Possible causes:
  // - Text too long for selected version
  // - Not enough memory
}
```

### Centering QRCode

```cpp
char text[] = "https://youtu.be/Vx5prDjKAcw";

bool success = qrcode.generateData(text);
if (success) {
  int16_t sideLength = qrcode.getSideLength();

  int16_t x = (display.width() - sideLength) / 2;
  int16_t y = (display.height() - sideLength) / 2;

  qrcode.draw(x, y);
}
```

### Drawing a QRCode Multiple Times

QR Code generation can take some time. If you intend to keep drawing the same QR Code over time, you can pass a flag to `draw` method to keep the data instead of releasing it.

```cpp
char text[] = "https://youtube.com/iW_Ct49H1ng";

qrcode.generateData(text);

qrcode.draw(0,     0, false); // do not release QR Code data
qrcode.draw(120,   0, false); // do not release QR Code data
qrcode.draw(0,   150, true);  // ok, release it now (free allocated memory)

qrcode.draw(0, 250, false); // nothing is drawn here because the data was released
```

### Using Static Buffers

If you want to avoid dynamic memory allocations (which may cause heap fragmentation), you can create and provide your own static buffers. Give them an arbitrary size or use the qrcodegen macro (see example below) to allocate the necessary space for a specific maximum version.

But be careful: if these static buffers are too large, the device may run out of memory and your program will freeze or misbehave. Leaving the dynamic allocation to the library might be safer.

```cpp
// Calculate required buffer size for a specific version
// qrcodegen_BUFFER_LEN_FOR_VERSION is a C macro
// That means you can use it to allocate a static variable at compile time
#define MAX_VERSION 5
static uint8_t qrcodeBuffer[qrcodegen_BUFFER_LEN_FOR_VERSION(MAX_VERSION)];
static uint8_t tempBuffer[qrcodegen_BUFFER_LEN_FOR_VERSION(MAX_VERSION)];

// Configure generator to use static buffer
qrcode.getGenerator().setBuffers(qrcodeBuffer, tempBuffer, sizeof(qrcodeBuffer));

// The library will automatically check if buffer size is enough for the text
String text = "https://youtu.be/EzWNBmjyv7Y";
bool success = qrcode.draw(text, 20, 50);
if (!success) {
  // Text too long for our buffer
}
```

### Memory Optimization
For devices with very limited memory (like Arduino Uno), you can enable memory optimization by defining `QRCODE_GENERATOR_USE_LESS_MEMORY` in `QRCodeGenerator.h`:

```cpp
// QRCode minimum version detection uses some big static variables as tables
// That might be a problem for some devices with low memory (e.g. Arduino Uno)
// You can save memory by defining QRCODE_GENERATOR_USE_LESS_MEMORY here (uncomment the line below)
// This will disable the tables and some methods at compile time, reducing memory usage
// In that case, you must set the version range according to your device capabilities
// Please check the text size limits for each version at the link below
// https://github.com/ricmoo/QRCode/tree/master?tab=readme-ov-file#data-capacities

#define QRCODE_GENERATOR_USE_LESS_MEMORY
```

In that case, you must manually set the version range according to your device capabilities:

```cpp
qrcode.getGenerator().setVersionRange(1, 6);
```

Additional examples can be found in the examples folder.

## Acknowledgment

The QRCode data generation is handled by [qrcodegen C library](https://github.com/nayuki/QR-Code-generator/tree/master/c), which is included in this repository.

## Contributing

Feel free to open issues and submit pull requests. All contributions are welcome!

## License

This library is released under the MIT License. See the LICENSE file for details.