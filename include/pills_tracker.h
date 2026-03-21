#ifndef PILLS_TRACKER
#define PILLS_TRACKER

#include "time_manager.h"
#include "eeprom_service.h"
#include "Buzzer.h"
#include "Time_manager.h"

class PillsTracker {
    public:
        PillsTracker() = default;
        void onLoop();
        void onPillTaken();
        void notifyPillTaken();
        void savePillTaken();

    private:
        bool morningPillTaken = false;
        bool eveningPillTaken = false;
};

#endif // PILLS_TRACKER