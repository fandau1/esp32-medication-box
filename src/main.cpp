#include <Arduino.h>
#include "bluetooth_service.h"
#include "eeprom_service.h"
#include "communication_manager.h"
#include <ArduinoJson.h>

BluetoothService bleService;
EEPROMService eepromService;
CommunicationManager commManager(bleService, eepromService);

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  eepromService.begin();

  bleService.begin("ESP32_SENSOR");
  commManager.begin();

  Serial.println("BLE ready – ESP32_SENSOR");
}

void loop() {

}