# 💡 Light Control Engine

![Status](https://img.shields.io/badge/status-stable-brightgreen?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-ESP32--S3%20%7C%20PlatformIO%2FArduino-green?style=flat-square)
![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen?style=flat-square)

> **A lightweight RGB lighting control engine for the ESP32-S3, operated entirely through a Serial interface.**

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/user-attachments/assets/b727d80a-9e4f-4a8d-9a36-24389212e693">
  <img src="https://github.com/user-attachments/assets/9fb87544-eb19-4efe-98e2-f9af064e90eb" alt="Light Control Engine banner">
</picture>

----

## 📑 Table of Contents

- [Overview](#overview)
- [Version 5.0 — Physical imersion](#version-5)
- [Configuration](#configuration)
- [Dependencies](#dependencies)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)


---
<a id="overview"></a>
## Overview

The ESP32-S3 onboard RGB LED is driven by a **single pin (GPIO 48)** using the NeoPixel protocol — no separate R/G/B pins required.

RGB color is represented by three channels, each ranging from `0` to `255`:

| Channel | Range | Description |
| ------- | ----- | ----------- |
| R       | 0–255 | Red         |
| G       | 0–255 | Green       |
| B       | 0–255 | Blue        |

Combined, this yields **16,777,216** possible colors (`256 × 256 × 256`).

The two core calls that drive the LED:

```c++
led.setPixelColor(0, led.Color(r, g, b));  // stage the color
led.show();                                 // apply it physically
```

---
> [!NOTE]
> I removed previous versions notes.

<a id="version-5"></a>
## 🌅 Version 5.0 — Physical imersion



> [!IMPORTANT]
> The full CLI reference for 4.0 is in [cli.md](cli.md).

---

<a id="configuration"></a>
## ⚙️ Configuration

```cpp
#define LED_PIN      48        // Onboard NeoPixel pin
#define LED_COUNT    1         // Number of LEDs
#define NAME_LEN     32        // Max length of a color name (with the terminator)
#define MAX_CUSTOM   20        // Max custom colors
#define MAX_FADES    30        // Max saved fades
#define MAX_TIME_MS  3600000UL // Longest time you can type (1h)
#define FADE_GAMMA   2.2       // Gamma exponent used by fadeg
```

The Serial baud rate is `115200`, set directly in `setup()`.

> [!TIP]
> `MAX_CUSTOM` and `MAX_FADES` can be changed freely.

> [!CAUTION]
> Changing `NAME_LEN` after you already saved colors changes the saved layout and can make old entries unreadable. If that happens, `array delete all` gets you back to a clean state.

---

<a id="dependencies"></a>
## 📦 Dependencies

- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) — LED driver
- `Preferences` — NVS storage, included in the ESP32 Arduino core

> [!IMPORTANT]
> Library versions are not pinned. If a dependency updates and breaks the build, lock versions in your package manager.

---


<a id="roadmap"></a>
## Roadmap

> [!NOTE]
> Items marked ✅ or 🚧 are (partly) implemented. Everything else is a planning list for future versions, not current behavior.

| Milestone | Target                                                        | Status         |
| :-------: | :------------------------------------------------------------ | :------------: |
| M1        | Persist colors in EEPROM/NVS (survives reboot)                | ✅ Done         |
| M2        | Support for multiple LEDs / addressable WS2812 strips         | ✅ Done(Coming in 5.0)      |
| M4        | Support for multiple LEDs /                                   | ✅ Done(Coming in 5.0)     |
| M5        | Animation effects (fade, rainbow, blink)                      | ✅ Done(fade already in, rest of it in 5.0)  |
| M7        | Control over WiFi/MQTT (in addition to Serial)                | 🔲 Planned      |
| M8        | Be able to change the fade graph curve                        | ✅ Done(Coming out in 5.0)      |
| M9        | Control over other peripherals(servo, motor, relay and etc)    | 🔲 Planned (coming in 6.0)
| M10       | Be able to attach sd card for almost unlimited storage        | 🔲 Planned (coming in 6.0)
| M11       | Be able to attach external processor for better rending NeoPixel strips | 🔲 Planned (coming in 6.0)


M1 landed in 3.0 (colors persist; brightness was removed in 4.0). M3 started with fades in 4.0, rainbow and blink are still to come.

---


<a id="contributing"></a>
## Contributing

Contributions are very welcome — new color modes, CLI improvements, docs, anything.

1. Fork the repo
2. Create a feature branch: `git checkout -b feat/your-feature`
3. Commit your changes: `git commit -m "feat: describe your change"`
4. Push and open a Pull Request against `main`

Please open an issue first for anything larger than a bug fix, so we can discuss direction before you invest time building it.

> [!IMPORTANT]
> When contributing firmware changes, always test on real hardware before submitting a PR. Serial timing and LED/fade behavior can differ from simulated builds.

---

<a id="license"></a>
## License

This project is licensed under the **MIT License**. See [LICENSE](./LICENSE) for the full text.
