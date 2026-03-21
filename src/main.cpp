#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CHAR_UUID_TX "12345678-1234-1234-1234-1234567890ac"
#define CHAR_UUID_RX "12345678-1234-1234-1234-1234567890ad"
#define LDR_PIN 32

BLECharacteristic *txChar;
bool connected = false;

class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer*)    { connected = true;  Serial.println("Připojeno"); }
  void onDisconnect(BLEServer*) { connected = false; Serial.println("Odpojeno");
    BLEDevice::getAdvertising()->start(); // automaticky znovu inzeruje
  }
};

class RxCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *c) {
    String val = String(c->getValue().c_str());
    Serial.println("Přijato: " + val);
  }
};

void sendBLE(String msg) {
  txChar->setValue(msg.c_str());
  txChar->notify();
  Serial.println("Odesláno: " + msg);
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  BLEDevice::init("ESP32_SENSOR");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCB());

  BLEService *svc = server->createService(SERVICE_UUID);

  txChar = svc->createCharacteristic(
    CHAR_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  txChar->addDescriptor(new BLE2902());

  BLECharacteristic *rxChar = svc->createCharacteristic(
    CHAR_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  rxChar->setCallbacks(new RxCB());

  svc->start();
  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  BLEDevice::getAdvertising()->start();

  Serial.println("BLE ready – ESP32_SENSOR");
}

void loop() {
  int ldr = analogRead(LDR_PIN);

  if (connected) {
    // Plaintext
    sendBLE("LDR:" + String(ldr));

    // Nebo JSON – vyber jedno
    // sendBLE("{\"ldr\":" + String(ldr) + "}");
  }

  Serial.println("LDR: " + String(ldr));
  delay(2000);
}