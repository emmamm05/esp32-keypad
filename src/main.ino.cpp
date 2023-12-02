# 1 "/var/folders/nr/44wtvtfd0kx0k_18b3jb_1xm0000gq/T/tmpcx8esow4"
#include <Arduino.h>
# 1 "/Users/emmanuel.mora/dev/esp32-keyboard/src/main.ino"
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;
int i;
int oldValue;
int values[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
const int pins[8] = {13, 14, 26, 33, 12, 27, 25, 32};
const int keys[8] = {
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24
};
void setup();
void loop();
#line 19 "/Users/emmanuel.mora/dev/esp32-keyboard/src/main.ino"
void setup() {
  for (i = 0; i < 8; i++){
    pinMode(pins[i], INPUT_PULLUP);
  }

  Serial.begin(115200);
  Serial.println("Starting!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    for (i = 0; i < 8; i++){
      oldValue = values[i];
      values[i] = digitalRead(pins[i]);
      if (values[i] == LOW && oldValue == HIGH) {
        bleKeyboard.press(keys[i]);
      } else if (values[i] == HIGH && oldValue == LOW) {
        bleKeyboard.release(keys[i]);
      }
    }
  }
  delay(50);
}