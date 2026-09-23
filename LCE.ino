#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define LED_COUNT 1

Adafruit_NeoPixel rgbLed(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// named colors bc typing 255 0 0 every time is annoying
struct NamedColor {
  const char* name;
  uint8_t r, g, b;
};

NamedColor colors[] = {
  {"red",     255, 0,   0  },
  {"green",   0,   255, 0  },
  {"blue",    0,   0,   255},
  {"yellow",  255, 255, 0  },
  {"cyan",    0,   255, 255},
  {"magenta", 255, 0,   255},
  {"white",   255, 255, 255},
  {"orange",  255, 165, 0  },
  {"purple",  128, 0,   128},  // <--- just like this
  {"off",     0,   0,   0  }   //add a coma here if u want to add another color
};

// lowercase a string in place
void toLowerStr(char* str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;  //this block was a bitch to write, why so complicated, don't ask me how ts works
  }
}

void setColor(int r, int g, int b) {
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  rgbLed.setPixelColor(0, rgbLed.Color(r, g, b));
  rgbLed.show();

  Serial.print("Color set to: ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.println(b);
}

void setup() {
  Serial.begin(115200);

  rgbLed.begin();
  rgbLed.clear();
  rgbLed.show();

  Serial.println("RGB LED Control");
  delay(500);
  Serial.println("Enter a rgb code, hex or color name");
  Serial.println("Example: 255 255 255  or  yellow  or  #FF5500");
}

void loop() {
  if (Serial.available() > 0) {
    char input[32];
    int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
    input[len] = '\0';

    // trim trailing \r if any
    if (len > 0 && input[len - 1] == '\r') input[--len] = '\0';

    toLowerStr(input);

    // check if its a named color
    bool found = false;
    int numColors = sizeof(colors) / sizeof(colors[0]);
    for (int i = 0; i < numColors; i++) {
      if (strcmp(input, colors[i].name) == 0) {
        setColor(colors[i].r, colors[i].g, colors[i].b);
        found = true;
        break;
      }
    }
//hex support block code
    if (!found && input[0] == '#') {
      int r, g, b;
      if (sscanf(input + 1, "%02x%02x%02x", &r, &g, &b) == 3) {
        setColor(r, g, b);
        found = true;
      } else {
        Serial.println("bad hex");  // u sure thats a valid hex bro
      }
    }

    if (!found) {
      int r = 0, g = 0, b = 0;
      // sscanf is cleaner here tbh
      if (sscanf(input, "%d %d %d", &r, &g, &b) == 3) {
        setColor(r, g, b);
      } else {
        Serial.println("Not recognised");  // didnt understand input, i wanted to put "wat" on the output, but the society would judge me😪
      }
    }

    while (Serial.available() > 0) Serial.read();
  }
}

// so i asked claude help to map all of the colors codes, so u don't really need to put the code in all of the colors, surely, you will have to search the internet if u want a flying chihuahua of the sahara desert blue, but the main colors are there
//u can still add colors just by following the pattern, so if u want to add the Goose Turd Green, just put the coma in the last array and add "{"Goose Turd Green",  84, 255, 162}
