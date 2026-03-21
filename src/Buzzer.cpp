#include "Buzzer.h"
#include "Arduino.h"

void Buzzer::begin(int pin) {
    buzzer_pin = pin;
    pinMode(buzzer_pin, OUTPUT);
}

void Buzzer::buzz(int count, unsigned long duration) {
    buzz(count, duration, duration);
}

void Buzzer::buzz(int count, unsigned long duration, unsigned long pause) {
    for (int i = 0; i < count; i++) {
        digitalWrite(buzzer_pin, HIGH);
        delay(duration);
        digitalWrite(buzzer_pin, LOW);
        delay(pause);
    }
}