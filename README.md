# LOCOBoard

LOCOBoard is an Arduino UNO R3 shield for education with the following features:

- 8x8 LED matrix
- 4x RGB address LEDs
- buzzer
- IMU
- IR remote receiver
- user-controlled button
- 2x DC motor connectors
- 3x servo motor connectors
- 3x laser distance sensor connectors
- SSD1306 screen connector
- bluetooth module connector
- I2C connector

# Hardware

## Schematics & components

## Usage

locoboard can be powered with any DC voltage power source providing between X and Y V.

# Software

For convenience, we provide a library with a custom API for all the LOCOBoard features. However, you can of course control individual hardware components using any compatible Arduino libraries.

## API reference

## Examples

## Dependencies

| Library | Version | Link |
| --- | --- | --- |
| Servo | 1.2.2 | [ZIP](https://github.com/arduino-libraries/Servo/archive/refs/tags/1.2.2.zip) |
| Arduino-IRremote | 4.4.0 | [ZIP](https://github.com/Arduino-IRremote/Arduino-IRremote/archive/refs/tags/v4.4.0.zip) |
| Adafruit_SSD1306 | 2.5.10 | [ZIP](https://github.com/adafruit/Adafruit_SSD1306/archive/refs/tags/2.5.10.zip) |