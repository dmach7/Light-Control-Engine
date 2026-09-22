
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define LED_COUNT 1

Adafruit_NeoPixel rgbLed(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  rgbLed.begin();
  rgbLed.clear();
  rgbLed.show();

  Serial.println("RGB LED Control");
  delay(500);
  Serial.println("Enter a rgb code");
  Serial.println("Example: 255 255 255 | No comas");
  
}

void loop() {
  if (Serial.available() > 0) {

    //Read input from serial
    int redValue = Serial.parseInt();
    int greenValue = Serial.parseInt();
    int blueValue = Serial.parseInt();

    //doesn't let user trespass the commom value
    redValue = constrain(redValue, 0, 255);
    greenValue = constrain(greenValue, 0, 255);
    blueValue = constrain(blueValue, 0, 255);

    //Set it
    rgbLed.setPixelColor(0, rgbLed.Color(
      redValue,
      greenValue,
      blueValue
    ));

    rgbLed.show();

    Serial.print("Color set to: ");
    Serial.print(redValue);
    Serial.print(", ");
    Serial.print(greenValue);
    Serial.print(", ");
    Serial.println(blueValue);

    while (Serial.available() > 0) {
      Serial.read();
    }
  }

##As i previously said on readme it as very raw code, just the core, it only change colors only this
}

