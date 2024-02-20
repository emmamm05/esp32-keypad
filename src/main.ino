#include <BleKeyboard.h>

#define KEYS_COUNT 4
#define KEY_PRESSED LOW
#define KEY_RELEASED HIGH
#define USE_NIMBLE

BleKeyboard bleKeyboard;

const int PINS[KEYS_COUNT] = {27, 13, 12, 14};
const int KEYS[KEYS_COUNT] = {0x01, 0x02, 0x03, 0x04};
int pin_values[KEYS_COUNT] = {HIGH, HIGH, HIGH, HIGH};
int i, prev_pin_val;

void setup() {
  for (i = 0; i < KEYS_COUNT; i++){
    pinMode(PINS[i], INPUT_PULLUP);
  }

  Serial.begin(115200);
  Serial.println("Starting!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    for (i = 0; i < KEYS_COUNT; i++){
      prev_pin_val = pin_values[i];
      pin_values[i] = digitalRead(PINS[i]);
      if (pin_values[i] == KEY_PRESSED && prev_pin_val == KEY_RELEASED) {
        bleKeyboard.press(KEYS[i]);
      } else if (pin_values[i] == KEY_RELEASED && prev_pin_val == KEY_PRESSED) {
        bleKeyboard.release(KEYS[i]);
      }
    }
  }
  delay(10);
}
