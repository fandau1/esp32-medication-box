#include "Buzzer.h"
#include "Arduino.h"

int Buzzer::buzzer_pin_ = -1;
bool Buzzer::is_alarm_on = false;

void Buzzer::begin(int pin) {
    buzzer_pin_ = pin;
    pinMode(buzzer_pin_, OUTPUT);
    digitalWrite(buzzer_pin_, LOW);
}

void Buzzer::buzz(int count, unsigned long duration) {
    buzz(count, duration, duration);
}

void Buzzer::buzz(int count, unsigned long duration, unsigned long pause) {
    if (buzzer_pin_ < 0) {
        return;
    }
    for (int i = 0; i < count; i++) {
        digitalWrite(buzzer_pin_, HIGH);
        delay(duration);
        digitalWrite(buzzer_pin_, LOW);
        delay(pause);
    }
}

void Buzzer::turnAlarmOn() {
    if (buzzer_pin_ < 0) {
        return;
    }
    digitalWrite(buzzer_pin_, HIGH);
    is_alarm_on = true;
}

void Buzzer::turnAlarmOff() {
    if (buzzer_pin_ < 0) {
        return;
    }
    digitalWrite(buzzer_pin_, LOW);
    is_alarm_on = false;
}

bool Buzzer::isAlarmOn() {
    return is_alarm_on;
}