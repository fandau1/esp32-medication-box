#ifndef EEPROM_SERVICE_H
#define EEPROM_SERVICE_H

#include <EEPROM.h>
#include "communication_types.h"

#define EEPROM_SIZE 512

class EEPROMService {
private:
    const int CONFIG_ADDR = 0;

public:
    void begin();
    void saveConfiguration(const MedicationConfiguration& config);
    MedicationConfiguration loadConfiguration();
};

#endif // EEPROM_SERVICE_H
