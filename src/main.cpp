#include <Arduino.h>
#include "bluetooth_service.h"

#define LDR_PIN 32
#define LED_EXTERNAL_PIN 4
#define BUTTON_PIN 14
#define BUZZER_PIN 18

BluetoothService bleService;

int ldr = 0;
unsigned long previousMillis = 0;
const long interval = 2000;
bool ledState = LOW;

unsigned long buttonPressTime = 0;
bool isBuzzing = false;

void onBleReceive(const std::string& value) {
  Serial.println("Přijato: " + String(value.c_str()));
}

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  bleService.begin("ESP32_SENSOR");
  bleService.setOnReceive(onBleReceive);

  Serial.println("BLE ready – ESP32_SENSOR");

  pinMode(LED_EXTERNAL_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(BUTTON_PIN) == LOW && !isBuzzing) {
    isBuzzing = true;
    buttonPressTime = currentMillis;
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (isBuzzing && (currentMillis - buttonPressTime >= 100)) {
    digitalWrite(BUZZER_PIN, LOW);
    isBuzzing = false;
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ldr = analogRead(LDR_PIN);

    if (bleService.isConnected()) {
      String json = "{\"ldr\":" + String(ldr) + "}";
      bleService.sendJson(json);
      Serial.println("Odesláno: " + json);
    }

    if (ledState == LOW) {
      ledState = HIGH;
      Serial.println("Rozsvícení LED: " + String(ldr));
    } else {
      ledState = LOW;
      Serial.println("Zhasnutí LED");
    }
    digitalWrite(LED_EXTERNAL_PIN, ledState);
    Serial.println("LDR: " + String(ldr));
  }
}