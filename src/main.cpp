#include <Arduino.h>
#include "bluetooth_service.h"
#include "eeprom_service.h"
#include "communication_manager.h"
#include <ArduinoJson.h>
#include "Buzzer.h"
#include "Button.h"

#define LED_EXTERNAL_PIN 4
#define BUTTON_PIN 14
#define BUZZER_PIN 18

BluetoothService bleService;
EEPROMService eepromService;
CommunicationManager commManager(bleService, eepromService);
Buzzer buzzer;
Button button;

void checkButton() {
  Event event = button.getEvent();
  switch (event) {
    case Event::None:
      return;
    case Event::SinglePress:
      buzzer.buzz(1, 300, 0);
      break;
    case Event::LongPress:
      buzzer.buzz(1, 1000, 0);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  eepromService.begin();

  bleService.begin("ESP32_SENSOR");
  commManager.begin();
  buzzer.begin(BUZZER_PIN);
  button.setPin(BUTTON_PIN);



  Serial.println("BLE ready – ESP32_SENSOR");

  pinMode(LED_EXTERNAL_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  button.readButton();    
  checkButton();
}