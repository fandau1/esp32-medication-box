#include <Arduino.h>
#include "bluetooth_service.h"
#include "eeprom_service.h"
#include "communication_manager.h"
#include <ArduinoJson.h>
#include "Buzzer.h"
#include "Button.h"
#include "pills_tracker.h"

#define BUTTON_PIN 14
#define BUZZER_PIN 18

#define LED_MORNING_PIN 4 
#define LED_EVENING_PIN 5

BluetoothService bleService;
CommunicationManager commManager(bleService);
Button button;
PillsTracker pilstracker;

void checkButton() {
  Event event = button.getEvent();
  switch (event) {
    case Event::None:
      return;
    case Event::SinglePress:
      // user take the pill
      pilstracker.onPillTaken();
      Buzzer::buzz(1, 300, 0);
      break;
    case Event::LongPress:
      // user reset the pills states
      Buzzer::buzz(1, 1000, 0);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  EEPROMService::begin();

  bleService.begin("ESP32_SENSOR");
  commManager.begin();
  Buzzer::begin(BUZZER_PIN);
  button.setPin(BUTTON_PIN);

  pinMode(LED_MORNING_PIN, OUTPUT);
  pinMode(LED_EVENING_PIN, OUTPUT);

  Serial.println("BLE ready – ESP32_SENSOR");
}

void loop() {
  button.readButton();    
  checkButton();
  commManager.loop();
  pilstracker.onLoop();
}