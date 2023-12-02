#include <BleKeyboard.h>

BleKeyboard bleKeyboard;
int i;
int oldValue;
int values[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
const int pins[8] = {13, 14, 26, 33, 12, 27, 25, 32};
const int keys[8] = {
  KEY_NUM_0,
  KEY_NUM_1,
  KEY_NUM_2,
  KEY_NUM_3,
  KEY_NUM_4,
  KEY_NUM_5,
  KEY_NUM_6,
  KEY_NUM_7
};

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
        bleKeyboard.write(keys[i]);
      }
    }
  }
  delay(50);
}
