#include <Arduino.h>
#include "bluetooth_service.h"
#include "Buzzer.h"
#include "Button.h"

#define LED_EXTERNAL_PIN 4
#define BUTTON_PIN 14
#define BUZZER_PIN 18

BluetoothService bleService;
Buzzer buzzer;
Button button;

unsigned long previousMillis = 0;
const long interval = 2000;
int counter = 0;

void onBleReceive(const std::string& value) {
  Serial.println("Přijato: " + String(value.c_str()));
}


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

  bleService.begin("ESP32_SENSOR");
  bleService.setOnReceive(onBleReceive);
  buzzer.begin(BUZZER_PIN);
  button.setPin(BUTTON_PIN);



  Serial.println("BLE ready – ESP32_SENSOR");

  pinMode(LED_EXTERNAL_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  buzzer.buzz(3, 200);
}

void loop() {
  unsigned long currentMillis = millis();
  button.readButton();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (bleService.isConnected()) {
      String json = "{\"ldr\":" + String(counter++) + "}";
      bleService.sendJson(json);
      Serial.println("Odesláno: " + json);
    }
  }
  
    
  checkButton();
}