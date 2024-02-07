#include <NimBLEDevice.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define KEYS_COUNT 8
#define KEY_PRESSED LOW
#define KEY_RELEASED HIGH

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

const int PINS[KEYS_COUNT] = {13, 14, 26, 33, 12, 27, 25, 32};
// const int KEYS[KEYS_COUNT] = {KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24};
const int KEYS[KEYS_COUNT] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
int pin_values[KEYS_COUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int i, prev_pin_val;

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */  
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
/***************** New - Security handled here ********************
****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest(){
    Serial.println("Server PassKeyRequest");
    return 123456; 
  }

  bool onConfirmPIN(uint32_t pass_key){
    Serial.print("The passkey YES/NO number: ");Serial.println(pass_key);
    return true; 
  }

  void onAuthenticationComplete(ble_gap_conn_desc desc){
    Serial.println("Starting BLE work!");
  }
/*******************************************************************/
};

void setup() {
  for (i = 0; i < KEYS_COUNT; i++){
    pinMode(PINS[i], INPUT_PULLUP);
  }

  Serial.begin(115200);
  Serial.println("Starting!");
   // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      NIMBLE_PROPERTY::READ   |
                      NIMBLE_PROPERTY::WRITE  |
                      NIMBLE_PROPERTY::NOTIFY |
                      NIMBLE_PROPERTY::INDICATE
                    );
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  /** Note, this could be left out as that is the default value */
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter

  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  if (deviceConnected) {
    for (i = 0; i < KEYS_COUNT; i++){
      prev_pin_val = pin_values[i];
      pin_values[i] = digitalRead(PINS[i]);
      if (pin_values[i] == KEY_PRESSED && prev_pin_val == KEY_RELEASED) {
        pCharacteristic->setValue((uint8_t*)&KEYS[i], 4);
        pCharacteristic->notify();
      } else if (pin_values[i] == KEY_RELEASED && prev_pin_val == KEY_PRESSED) {
        pCharacteristic->setValue((uint8_t*)&KEYS[i], 4);
        pCharacteristic->notify();
      }
    }
  }
  delay(50);
}
