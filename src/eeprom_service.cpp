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

void EEPROMService::savePillTaken(const time_t& time) {
    int freeIndex = EEPROMService::PILL_HISTORY_FIRST_ADDR + EEPROMService::PILL_HISTORY_COUNT * sizeof(time_t);
    for (int i = 0; i < sizeof(time_t); i++) {
        EEPROM.write(freeIndex + i, ((uint8_t*)&time)[i]);
    }
    EEPROM.commit();
}

String EEPROMService::loadPillTaken(size_t index) {
    time_t time;
    int addr = EEPROMService::PILL_HISTORY_FIRST_ADDR + index * sizeof(time_t);
    for (int i = 0; i < sizeof(time_t); i++) {
        ((uint8_t*)&time)[i] = EEPROM.read(addr + i);
    }
    return String(ctime(&time));
}
