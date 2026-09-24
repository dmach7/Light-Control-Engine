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

## Version 2.0

In the second version i brought some good updates!

Now you can choose a color WITHOUT having to type the rgb code.

Basically i added an array with maped colors, example red

```
NamedColor colors[] = {
  {"red",     255, 0,   0  },
```

i added this with a bunch of other colors so you don't really need to look like an neandertal searching for RGB codes! yayy

also added hex code support

```
#FF5500
#00FF88
```

input priority: the prompt follows a priority of commands:

```
named color → hex → rgb values → "Not recognised"
```
And also to avoid dum ahh user frustration i added a script that every system should have

It is the:
```toLowerStr```
this function allows almost any type of input, so u don't especifically need to write "Red" for example, u can type "red" or "ReD" or "RED", the function was pretty hard to write but it makes the system much better

Here is the code for the function

```
void toLowerStr(char* str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;  //this block was a bitch to write, why so complicated, don't ask me how ts works
  }
```

Ignore angry dev comment

## Version 3.0

For the 3.0 i'm adding the brightness function!

On the currently using library there's already a function for that, so the code it's basically done

> [!NOTE]
> I know that brightness isn't really in the rgb spectrum, so if you have white ```(255, 255, 255)``` and want to lower the brightness you can just do ```(100, 100, 1000)``` but to make it easier i'm adding it

The brightness function is bassically this:
```
// brightness block
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
Now i only need to find out where to put this on the actual code
