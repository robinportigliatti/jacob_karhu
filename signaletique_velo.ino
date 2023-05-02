#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 299
#define LED_PIN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_LIGHT 5
#define BUTTON_WARNING 6
#define BUTTON_STOP 7

#define COLOR_ORANGE 0Xff2200
#define COLOR_WHITE 0xFFFFFF
#define COLOR_RED 0xFF0000
#define COLOR_BLUE 0x31eded

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const unsigned long debounceDelay = 10;
unsigned long lastDebounceTime[] = {0, 0, 0, 0, 0};
int buttonState[] = {0, 0, 0, 0, 0};
int lastButtonState[] = {0, 0, 0, 0, 0};
int buttons[] = {BUTTON_LEFT, BUTTON_RIGHT, BUTTON_LIGHT, BUTTON_WARNING, BUTTON_STOP};

void setup() {
  strip.begin();
  strip.show();
  for (int i = 0; i < 5; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < 5; i++) {
    int reading = digitalRead(buttons[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if (millis() - lastDebounceTime[i] > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == LOW) {
          handleButtonPress(i);
        }
      }
    }
    lastButtonState[i] = reading;
  }
}

void handleButtonPress(int buttonIndex) {
  switch (buttonIndex) {
    case 0:
      BlinkLeft();
      break;
    case 1:
      BlinkRight();
      break;
    case 2:
      Light();
      break;
    case 3:
      Warning();
      break;
    case 4:
      Stop();
      break;
  }
}

void BlinkLeft() {
  blink(COLOR_ORANGE, 0, 30, 180, 210, -1);
}

void BlinkRight() {
  blink(COLOR_ORANGE, 30, 60, 150, 180, 1);
}

void blink(uint32_t color, int start1, int end1, int start2, int end2, int step) {
  for (int a = 0; a < 5; a++) {
    for (int i = start1, j = start2; i != end1 && j != end2; i += step, j -= step) {
      strip.setPixelColor(i, color);
      strip.setPixelColor(j, color);
      strip.show();
      delay(25);
    }
    Off();
    delay(50);
  }
}

void Warning() {
  for (int i = 0; i < 2; i++) {
    setColorAll(COLOR_ORANGE);
    delay(500);
    Off();
    delay(500);
  }
}

void Light() {
  for (int b = 0, d = 299; b < 30 && d > 269; b++, d--) {
    strip.clear();
    for (int c = b, e = d; c < 150 && e > 150; c += 30, e -= 30) {
      strip.setPixelColor(c, COLOR_BLUE);
      strip.setPixelColor(e, COLOR_BLUE);
    }
    strip.show();
    delay(50);
  }
}

void Stop() {
  setColorRange(COLOR_RED, 0, 60);
}

void Off() {
  setColorAll(0);
}

void setColorAll(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setColorRange(uint32_t color, int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
