# Light Control Engine

A lightweight RGB lighting control engine for the ESP32-S3.

## Version 1.0

The first version focuses on basic RGB color control through the
Serial interface.

The ESP32-S3 development board includes a small onboard RGB LED.
RGB color is represented by three channels:

- **R** — Red
- **G** — Green
- **B** — Blue

Each channel can have a value from 0 to 255, resulting in:
256³ = 16.777.216 colors

so for example here are some common color codes

RGB(100, 100, 100) → Low-intensity white
RGB(255, 255, 255) → Maximum-intensity white
RGB(255, 0, 0)     → Red
RGB(0, 255, 0)     → Green
RGB(0, 0, 255)     → Blue

Ok so i did some research and i'll need to use the ESP32-S3-Box board on arduino IDE, and the main code that makes the rgb led blinks is:

´´´led.setPixelColor(0, led.Color(r, g, b));´´´
