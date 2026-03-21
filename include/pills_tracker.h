#ifndef PILLS_TRACKER
#define PILLS_TRACKER

#include "time_manager.h"
#include "eeprom_service.h"

enum class DayMedicationState {
    BEFORE_ALARM,
    AT_ALARM,
    AFTER_ALARM,
    MEDICATION_COLLECTED
};

class PillsTracker {
    public:
        PillsTracker(EEPROMService eeprom_service);
        void onPillTaken();
        void onLoop();

    private:
        DayMedicationState morningState;
        DayMedicationState eveningState;
        EEPROMService eeprom_service;
};

#endif // PILLS_TRACKER