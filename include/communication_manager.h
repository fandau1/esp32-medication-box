#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "bluetooth_service.h"
#include "eeprom_service.h"
#include <string>

class CommunicationManager {
public:
    CommunicationManager(BluetoothService& bleService, EEPROMService& eepromService);
    void begin();
    void onBleReceive(const std::string& value);

private:
    BluetoothService& bleService;
    EEPROMService& eepromService;
};

#endif // COMMUNICATION_MANAGER_H
