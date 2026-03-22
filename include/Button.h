#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>
#include "config.h"

enum class Event {
    None = 0,
    LongPress,
    SinglePress,
};

class Button {
 public:

    void setPin(uint8_t pin);
    Event getEvent();
    void readButton();

 private:
	int button_pin;
    Event last_event = Event::None;

    bool multiPress = false;
    bool pressed;
    unsigned long last_press_time = 0;
    unsigned long holding_time = 0;
};

#endif // BUTTON_H

