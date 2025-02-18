# QRCodeGFX

QRCodeGFX is an Arduino library for drawing QR codes on displays. It is designed to be display-agnostic and works with any library that inherits from Adafruit_GFX.

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
  
  // Draw a black and white QR code
  // Parameters:
  //   text: content to encode
  //   x: horizontal position (upper left corner)
  //   y: vertical position (upper left corner)
  qrcode.draw("https://youtu.be/dQw4w9WgXcQ", 10, 10);
}

void loop() {
}
```

### Customized Drawing
```cpp
// Configure appearance using method chaining
qrcode.setScale(3)                     // from 1 to 20
      .setColors(TFT_YELLOW, TFT_BLUE) // background, foreground
      .setRotation(QRCodeRotation::R90);   // 0°, 90°, 180°, or 270°

// Configure qrcode generation
qrcode.getGenerator()
      .setErrorCorrectionLevel(QRCodeECCLevel::High)
      .setVersion(6); // force specific version

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

int16_t sideLength = qrcode.generate(text);

int16_t x = (display.width() - sideLength) / 2;
int16_t y = (display.height() - sideLength) / 2;

qrcode.draw(x, y);
```

### Drawing QRCode Multiple Times

```cpp
char text[] = "https://youtube.com/iW_Ct49H1ng";

qrcode.generate(text);

qrcode.draw(0, 0, false);   // do not free allocated memory
qrcode.draw(0, 50, false); // do not free allocated memory
qrcode.draw(0, 100, true);  // now free allocated memory

qrcode.draw(0, 150, true);  // nothing is drawn here, since memory was freed
```

Check out more examples in the `examples` folder.

## Acknowledgment

The QRCode data generation is handled by [qrcodegen C library](https://github.com/nayuki/QR-Code-generator/tree/master/c), which was included in this repository.

## Contributing

Feel free to open issues and submit pull requests. All contributions are welcome!

## License

This library is released under the MIT License. See the LICENSE file for details.