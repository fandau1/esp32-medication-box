#ifndef EEPROM_SERVICE_H
#define EEPROM_SERVICE_H

#include <EEPROM.h>
#include <time.h>
#include "communication_types.h"

#define EEPROM_SIZE 512

class EEPROMService {
private:
    static const int CONFIG_ADDR = 0;
    static const int PILL_HISTORY_COUNT = sizeof(MedicationConfiguration);
    static const int PILL_HISTORY_FIRST_ADDR = sizeof(MedicationConfiguration) + sizeof(int);

public:
    static void begin();
    static void saveConfiguration(const MedicationConfiguration& config);
    static MedicationConfiguration loadConfiguration();

    static void savePillTaken(const time_t& time);
    static String loadPillTaken(size_t index);
};

#endif // EEPROM_SERVICE_H
