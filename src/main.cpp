#include <Arduino.h>
#include "bluetooth_service.h"
#include "eeprom_service.h"
#include "communication_manager.h"
#include <ArduinoJson.h>
#include "Buzzer.h"
#include "Button.h"
#include "pills_tracker.h"
#include "config.h"


BluetoothService bleService;
CommunicationManager commManager(bleService);
Button button;
PillsTracker pillsTracker(commManager);

void checkButton() {
  Event event = button.getEvent();
  switch (event) {
    case Event::None:
      return;
    case Event::SinglePress:
      Serial.println("Button single pressed");
      // user take the pill
      if (!Buzzer::isAlarmOn()) {
        Buzzer::buzz(1, 300, 0);
      }
      pillsTracker.onPillTaken();
      break;
    case Event::LongPress:
      Serial.println("Button long pressed");
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
  pillsTracker.onLoop();
}