#include "Buzzer.h"

#include <Arduino.h>

namespace {
constexpr uint32_t kAlarmOnMs = 200;
constexpr uint32_t kAlarmOffMs = 100;

inline TickType_t msToTicks(uint32_t ms) {
    return pdMS_TO_TICKS(ms);
}
} // namespace

int Buzzer::buzzer_pin_ = -1;
volatile bool Buzzer::is_alarm_on = false;

QueueHandle_t Buzzer::queue_ = nullptr;
TaskHandle_t Buzzer::task_handle_ = nullptr;

struct Buzzer::BuzzCommand {
    uint16_t count;
    uint32_t duration_ms;
    uint32_t pause_ms;
};

void Buzzer::begin(int pin) {
    buzzer_pin_ = pin;
    pinMode(buzzer_pin_, OUTPUT);
    digitalWrite(buzzer_pin_, LOW);

    is_alarm_on = false;

    if (queue_ == nullptr) {
        queue_ = xQueueCreate(8, sizeof(BuzzCommand));
    }

    if (task_handle_ == nullptr) {
        // Keep this task lightweight; priority 1 is enough for a buzzer.
        // Pin to core 0 to avoid interfering with common WiFi/BLE task affinity patterns.
        xTaskCreatePinnedToCore(Buzzer::taskMain, "BuzzerTask", 2048, nullptr, 1, &task_handle_, 0);
    }
}

void Buzzer::buzz(int count, unsigned long duration) {
    buzz(count, duration, duration);
}

void Buzzer::buzz(int count, unsigned long duration, unsigned long pause) {
    if (buzzer_pin_ < 0) {
        return;
    }

    // When alarm is active, the buzzer task generates the repeating pattern.
    // Ignore repeated buzz requests (e.g., from PillsTracker::onLoop()) to avoid queue flooding.
    if (is_alarm_on) {
        return;
    }

    if (queue_ == nullptr || task_handle_ == nullptr) {
        // Fallback (shouldn't happen if begin() was called), keep behavior compatible.
        for (int i = 0; i < count; i++) {
            digitalWrite(buzzer_pin_, HIGH);
            delay(duration);
            digitalWrite(buzzer_pin_, LOW);
            delay(pause);
        }
        return;
    }

    BuzzCommand cmd{
        static_cast<uint16_t>(count < 0 ? 0 : count),
        static_cast<uint32_t>(duration),
        static_cast<uint32_t>(pause),
    };

    // Non-blocking enqueue; if queue is full, drop the request.
    (void)xQueueSend(queue_, &cmd, 0);
}

void Buzzer::turnAlarmOn() {
    if (buzzer_pin_ < 0) {
        return;
    }
    is_alarm_on = true;
}

void Buzzer::turnAlarmOff() {
    if (buzzer_pin_ < 0) {
        return;
    }
    is_alarm_on = false;
}

bool Buzzer::isAlarmOn() {
    return is_alarm_on;
}

void Buzzer::taskMain(void* /*pvParameters*/) {
    BuzzCommand cmd{};

    for (;;) {
        if (buzzer_pin_ < 0) {
            vTaskDelay(msToTicks(100));
            continue;
        }

        // Prefer executing explicit buzz commands; otherwise, fall back to alarm pattern.
        if (queue_ != nullptr && xQueueReceive(queue_, &cmd, msToTicks(50)) == pdTRUE) {
            // Execute the full sequence.
            for (uint16_t i = 0; i < cmd.count; i++) {
                digitalWrite(buzzer_pin_, HIGH);
                vTaskDelay(msToTicks(cmd.duration_ms));
                digitalWrite(buzzer_pin_, LOW);
                vTaskDelay(msToTicks(cmd.pause_ms));
            }
            continue;
        }

        if (is_alarm_on) {
            digitalWrite(buzzer_pin_, HIGH);
            vTaskDelay(msToTicks(kAlarmOnMs));
            digitalWrite(buzzer_pin_, LOW);
            vTaskDelay(msToTicks(kAlarmOffMs));
        } else {
            digitalWrite(buzzer_pin_, LOW);
            vTaskDelay(msToTicks(100));
        }
    }
}