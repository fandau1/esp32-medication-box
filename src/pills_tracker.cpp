#include "pills_tracker.h"

void PillsTracker::onLoop() {
    if(Buzzer::isAlarmOn()) {
        Buzzer::buzz(1, 200, 100);
        return;
    }
    int current_time_mins = Time_manager::getMinsInDay();
    MedicationConfiguration schedule = EEPROMService::loadConfiguration();
    int morningSchedule = schedule.morning.alert.hour * 60 + schedule.morning.alert.minute;
    int eveningSchedule = schedule.evening.alert.hour * 60 + schedule.evening.alert.minute;

    if (!morningPillTaken && current_time_mins >= morningSchedule + schedule.morning.interval_notify_minutes) {
        Buzzer::turnAlarmOn();
    }

    if (!eveningPillTaken && current_time_mins >= eveningSchedule + schedule.evening.interval_notify_minutes) {
        Buzzer::turnAlarmOn();
    }

}

void PillsTracker::onPillTaken() {
    if (Buzzer::isAlarmOn()) {
        Buzzer::turnAlarmOff();
    }
    if (!morningPillTaken) 
        morningPillTaken = true;
    else if (!eveningPillTaken)
        eveningPillTaken = true;
    else {
        Buzzer::buzz(7, 100, 100);
        return;
    }

    notifyPillTaken();
}

void PillsTracker::notifyPillTaken() {
    
    savePillTaken();
}

void PillsTracker::savePillTaken() {
    EEPROMService::savePillTaken(Time_manager::getRTC());
}

