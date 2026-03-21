#include "Button.h"

void Button::setPin(uint8_t pin) {
    button_pin = pin;
}

Event Button::getEvent() {
    Event event = last_event;
    if (!pressed) {
        last_event = Event::None;
    }
    return event;
}

void Button::readButton() {
        unsigned long current_time = millis();
        if (digitalRead(button_pin) == LOW) {
            if (!pressed) {
                pressed = true;
                last_press_time = current_time;
                holding_time = 0;
            } else {
                holding_time = current_time - last_press_time;
                if (holding_time >= LONG_PRESS_THRESHOLD) {
                    last_event = Event::LongPress;
                }
            }
        } else {
            if (pressed) {
                pressed = false;
                if (holding_time >= LONG_PRESS_THRESHOLD) {
                    last_event = Event::LongPress;
                } else if (holding_time >= PRESS_THRESHOLD) {
                    last_event = Event::SinglePress;
                }
            }
        }
}

