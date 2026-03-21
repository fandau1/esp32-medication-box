#include "pills_tracker.h"

PillsTracker::PillsTracker(EEPROMService eeprom_service) : eeprom_service(eeprom_service) {
}

void PillsTracker::onPillTaken() {
    if(morningState == DayMedicationState::MEDICATION_COLLECTED) {
        if(eveningState == DayMedicationState::AT_ALARM || eveningState == DayMedicationState::AFTER_ALARM) {
            // pick evening medication
            eveningState = DayMedicationState::MEDICATION_COLLECTED;
        }
    } else {
        if(morningState == DayMedicationState::AT_ALARM || morningState == DayMedicationState::AFTER_ALARM) {
            morningState = DayMedicationState::MEDICATION_COLLECTED;
        }
    }
}

void PillsTracker::onLoop() {
    time_t current_time = Time_manager::getRTC();
    MedicationConfiguration config = eeprom_service.loadConfiguration();

    struct tm* timeinfo;
    timeinfo = localtime(&current_time);

    // Morning medication logic
    if (timeinfo->tm_hour < config.morning.alert.hour ||
        (timeinfo->tm_hour == config.morning.alert.hour && timeinfo->tm_min < config.morning.alert.minute)) {
        // Before morning alarm
        this->morningState = DayMedicationState::BEFORE_ALARM;
    } else if ((timeinfo->tm_hour > config.morning.alert.hour ||
                (timeinfo->tm_hour == config.morning.alert.hour && timeinfo->tm_min >= config.morning.alert.minute)) &&
               (timeinfo->tm_hour < (config.morning.alert.hour + (config.morning.alert.minute + config.morning.interval_notify_minutes) / 60)) ||
               (timeinfo->tm_hour == (config.morning.alert.hour + (config.morning.alert.minute + config.morning.interval_notify_minutes) / 60) && timeinfo->tm_min < ((config.morning.alert.minute + config.morning.interval_notify_minutes) % 60))) {
        // At morning alarm
        this->morningState = DayMedicationState::AT_ALARM;
    } else {
        // After morning alarm
        this->morningState = DayMedicationState::AFTER_ALARM;
    }

    // Evening medication logic
    if (timeinfo->tm_hour < config.evening.alert.hour ||
        (timeinfo->tm_hour == config.evening.alert.hour && timeinfo->tm_min < config.evening.alert.minute)) {
        // Before evening alarm
        this->eveningState = DayMedicationState::BEFORE_ALARM;
    } else if ((timeinfo->tm_hour > config.evening.alert.hour ||
                (timeinfo->tm_hour == config.evening.alert.hour && timeinfo->tm_min >= config.evening.alert.minute)) &&
               (timeinfo->tm_hour < (config.evening.alert.hour + (config.evening.alert.minute + config.evening.interval_notify_minutes) / 60)) ||
               (timeinfo->tm_hour == (config.evening.alert.hour + (config.evening.alert.minute + config.evening.interval_notify_minutes) / 60) && timeinfo->tm_min < ((config.evening.alert.minute + config.evening.interval_notify_minutes) % 60))) {
        // At evening alarm
        this->eveningState = DayMedicationState::AT_ALARM;
    } else {
        // After evening alarm
        this->eveningState = DayMedicationState::AFTER_ALARM;
    }
}