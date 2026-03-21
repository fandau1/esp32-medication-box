#include <Arduino.h>
#include "bluetooth_service.h"

#define LDR_PIN 32

BluetoothService bleService;

void onBleReceive(const std::string& value) {
  Serial.println("Přijato: " + String(value.c_str()));
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  bleService.begin("ESP32_SENSOR");
  bleService.setOnReceive(onBleReceive);

  Serial.println("BLE ready – ESP32_SENSOR");
}

void loop() {
  int ldr = analogRead(LDR_PIN);

  if (bleService.isConnected()) {
    String json = "{\"ldr\":" + String(ldr) + "}";
    bleService.sendJson(json);
    Serial.println("Odesláno: " + json);
  }

  Serial.println("LDR: " + String(ldr));
  delay(2000);
}