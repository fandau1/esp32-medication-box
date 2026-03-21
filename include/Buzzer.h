#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
public:
    Buzzer() = delete;
    static void begin(int pin);
    static void buzz(int count, unsigned long duration);
    static void buzz(int count, unsigned long duration, unsigned long pause);

    static void turnAlarmOn();
    static void turnAlarmOff();
    static bool isAlarmOn();

private:
    static int buzzer_pin_;
    static bool is_alarm_on;
};

#endif // BUZZER_H