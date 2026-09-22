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

Each channel can have a value from `0` to `255`, resulting in:

```256 × 256 × 256 = 16,777,216```

So after some test and research i found out that this board doesn't require 3 individual pins for RGB, it only uses one, which is the pin 48, i could finally find the code that actually switches the rgb light and it is:

```led.setPixelColor(0, led.Color(r, g, b));```

and this is the one line that makes the led physically apply the change for the led

```led.show();```

so this is the raw code, i'll need to create something that reads serial data and apply for the led. 
