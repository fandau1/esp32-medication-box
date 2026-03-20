#include <Arduino.h>

// Fotoresistor na GPIO32 (ADC1)

const int LDR_PIN = 32;

void setup() {
  Serial.begin(9600);

  // Volitelně zlepšení ADC přesnosti
  analogReadResolution(12); // 0–4095
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR hodnota: ");
  Serial.println(ldrValue);

  delay(500);
}