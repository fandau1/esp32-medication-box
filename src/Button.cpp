#include "Button.h"

void Button::setPin(uint8_t pin) {
    button_pin = pin;
    pinMode(button_pin, INPUT_PULLUP);
}

Event Button::getEvent() {
    Event event = last_event;
    last_event = Event::None;
    return event;
}

void Button::readButton() {
        unsigned long current_time = millis();
        if (digitalRead(button_pin) == LOW) {
            if (!pressed) {
                pressed = true;
                last_press_time = current_time;
            } else {
                holding_time = current_time - last_press_time;
                if (holding_time >= LONG_PRESS_THRESHOLD) {
                    last_event = Event::LongPress;
                    last_press_time = current_time;
                    multiPress = true;
                }
            }
        } else {
            if (pressed) {
                pressed = false;
                if (holding_time >= LONG_PRESS_THRESHOLD) {
                    last_event = Event::LongPress;
                } else if (holding_time >= PRESS_THRESHOLD && !multiPress) {
                    last_event = Event::SinglePress;
                }
                multiPress = false;
            }
        }
}

