# 💡 Light Control Engine

![Status](https://img.shields.io/badge/status-stable-brightgreen?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-ESP32--S3%20%7C%20PlatformIO%2FArduino-green?style=flat-square)
![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen?style=flat-square)

> **A lightweight RGB lighting control engine for the ESP32-S3, operated entirely through a Serial interface.**

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/user-attachments/assets/adacb953-3634-4590-8384-f0e306ec3d3a"">
  <img alt="Light Control Engine banner" src="https://github.com/user-attachments/assets/344011f8-3df6-45d9-aab3-54de69b98057)">
</picture>

---

## 📑 Table of Contents

- [Overview](#overview)
- [Version 1.0 — Basic RGB Control](#-version-10--basic-rgb-control)
- [Version 2.0 — Named Colors & Hex Support](#-version-20--named-colors--hex-support)
- [Version 3.0 — Brightness & CLI](#-version-30--brightness--cli)
- [Version 4.0 — Fades](#-version-40--fades)
- [Configuration](#%EF%B8%8F-configuration)
- [Dependencies](#-dependencies)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

---

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

## 🔴 Version 1.0 — Basic RGB Control

The first version establishes the foundation: read RGB values from Serial and apply them to the onboard LED.

**Input format:**

```
255 0 128
```

<details>
<summary><b>Flowchart view</b></summary>

```mermaid
flowchart TD
    A([Boot]) --> B[Init NeoPixel\nGPIO 48]
    B --> C[Wait for Serial input]
    C --> D[Parse R G B values]
    D --> E[setPixelColor]
    E --> F[show]
    F --> C
```

</details>

---

## 🎨 Version 2.0 — Named Colors & Hex Support

### Named Color Array

Instead of typing raw RGB codes, you can now input color names directly. Colors are mapped in a struct array:

```c++
NamedColor colors[] = {
  {"red",     255, 0,   0  },
  {"green",   0,   255, 0  },
  {"blue",    0,   0,   255} <- add comma here for another line
  // add more as needed
};
```

> [!TIP]
> You can extend the array with any custom color. Just follow the pattern — name, R, G, B.

### Hex Support

Standard hex color codes are also accepted:

```
#FF5500
#00FF88
```

### Case-Insensitive Input

All input is normalized to lowercase before parsing, so any of these work:

```
red / Red / RED / ReD
```

<details>
<summary><b>toLowerStr implementation</b></summary>

```c++
void toLowerStr(char* str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
  }
}
```

</details>

### Input Priority

When a command is received, the parser follows this resolution order:

```
named color → hex code → raw RGB values → "Not recognised"
```

<details>
<summary><b>Flowchart view</b></summary>

```mermaid
flowchart TD
    A([Serial Input]) --> B[toLowerStr]
    B --> C{Named color?}
    C -->|Yes| G[Apply color]
    C -->|No| D{Hex code?}
    D -->|Yes| G
    D -->|No| E{RGB values?}
    E -->|Yes| G
    E -->|No| F[Print: Not recognised]
    G --> H[setPixelColor]
    H --> I[show]
    I --> A
```

</details>

---

## 🔆 Version 3.0 — Brightness & CLI

> [!WARNING]
> Brightness was **removed in 4.0**. The brightness parts of this section are kept as history only and are marked below. See [Brightness removal](#-brightness-removal) in the 4.0 section.

### Brightness Control

Brightness was handled by the NeoPixel library directly, independent of the color channels.

> [!NOTE]
> Brightness and color were separate concerns. `setBrightness(100)` with `(255, 255, 255)` kept the color value intact for future reference, unlike manually scaling down the RGB values.

**Input format (removed in 4.0):**

```
brightness 180
```

<details>
<summary><b>Brightness implementation (removed in 4.0)</b></summary>

```++
if (!found && strncmp(input, "brightness ", 11) == 0) {
  int val = atoi(input + 11);
  val = constrain(val, 0, 255);
  rgbLed.setBrightness(val);
  rgbLed.show();
  Serial.print("Brightness set to: ");
  Serial.println(val);
  found = true;
}
```

</details>

> [!WARNING]
> Brightness values were not persisted across resets. The LED returned to default brightness on reboot.

### CLI Commands

> [!WARNING]
> Commands are case-sensitive and do not use the same normalisation as color names. Type them exactly as shown.

| Command        | Arguments                  | Description                   |
| -------------- | -------------------------- | ----------------------------- |
| `display`      | rgb code, hex code or name | Set the LED color             |
| `array add`    | name r g b                 | Add a new named color         |
| `array edit`   | name r g b                 | Edit an existing color        |
| `array rename` | old-name new-name          | Rename a color entry          |
| `array delete` | name                       | Remove a color from the array |

> [!IMPORTANT]
> Full CLI reference is documented in a separate file: [cli.md](cli.md).

<details>
<summary><b>Full input flowchart (3.0, brightness branch removed in 4.0)</b></summary>

```mermaid
flowchart TD
    A([Serial Input]) --> B{Command type?}
    B -->|display| C[Parse input]
    B -->|array| D[Parse array subcommand]
    B -->|brightness| E[Parse brightness value]
    B -->|unknown| F[Print: Not recognised]

    C --> G[toLowerStr]
    G --> H{Named color?}
    H -->|Yes| L[Apply color]
    H -->|No| I{Hex code?}
    I -->|Yes| L
    I -->|No| J{RGB values?}
    J -->|Yes| L
    J -->|No| F

    D --> K{Subcommand?}
    K -->|add| M[Append to array]
    K -->|edit| N[Update entry]
    K -->|rename| O[Rename entry]
    K -->|delete| P[Remove entry]

    E --> Q[constrain 0–255]
    Q --> R[setBrightness]
    R --> S[show]

    L --> T[setPixelColor]
    T --> S
    S --> A
```

</details>

---

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

## 📦 Dependencies

- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) — LED driver
- `Preferences` — NVS storage, included in the ESP32 Arduino core

> [!IMPORTANT]
> Library versions are not pinned. If a dependency updates and breaks the build, lock versions in your package manager.

---

## Roadmap

> [!NOTE]
> Items marked ✅ or 🚧 are (partly) implemented. Everything else is a planning list for future versions, not current behavior.

| Milestone | Target                                                        | Status         |
| :-------: | :------------------------------------------------------------ | :------------: |
| M1        | Persist colors in EEPROM/NVS (survives reboot)                | ✅ Done         |
| M2        | Support for multiple LEDs / addressable WS2812 strips         | 🔲 Planned      |
| M3        | Animation effects (fade, rainbow, blink)                      | 🚧 In progress  |
| M4        | Save/load custom color array via Serial command               | 🔲 Planned      |
| M5        | Control over WiFi/MQTT (in addition to Serial)                | 🔲 Planned      |

M1 landed in 3.0 (colors persist; brightness was removed in 4.0). M3 started with fades in 4.0, rainbow and blink are still to come.

---

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

## License

This project is licensed under the **MIT License**. See [LICENSE](./LICENSE) for the full text.
