#include "eeprom_service.h"

void EEPROMService::begin() {
    EEPROM.begin(EEPROM_SIZE);
}

void EEPROMService::saveConfiguration(const MedicationConfiguration& config) {
    EEPROM.put(CONFIG_ADDR, config);
    EEPROM.commit();
}

MedicationConfiguration EEPROMService::loadConfiguration() {
    MedicationConfiguration config;
    EEPROM.get(CONFIG_ADDR, config);
    return config;
}
