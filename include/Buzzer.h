#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

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
    struct BuzzCommand;

    static void taskMain(void* pvParameters);

    static int buzzer_pin_;
    static volatile bool is_alarm_on;

    static QueueHandle_t queue_;
    static TaskHandle_t task_handle_;
};

#endif // BUZZER_H