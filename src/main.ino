#include <BleKeyboard.h>
#include <BluetoothSerial.h>
#define KEYS_COUNT 8
#define KEY_PRESSED LOW
#define KEY_RELEASED HIGH
#define USE_NIMBLE

BleKeyboard bleKeyboard;
BluetoothSerial SerialBT;

const int PINS[KEYS_COUNT] = {13, 14, 26, 33, 12, 27, 25, 32};
const int KEYS[KEYS_COUNT] = {KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24};
int pin_values[KEYS_COUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
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
