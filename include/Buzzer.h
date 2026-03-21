#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
public:
    void begin(int pin);
    void buzz(int count, unsigned long duration);
    void buzz(int count, unsigned long duration, unsigned long pause);

private:
    int buzzer_pin;
};

#endif // BUZZER_H