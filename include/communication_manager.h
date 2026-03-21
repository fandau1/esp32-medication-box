#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "bluetooth_service.h"
#include "eeprom_service.h"
#include <string>
#include <time_manager.h>

class CommunicationManager {
public:
    CommunicationManager(BluetoothService& bleService);
    void begin();
    void loop();
    void onBleReceive(const std::string& value);
    bool outgoingPostMedicamentsTakenConfirmation();
    void outgoingPostHearthbeat();
    void outgoingGetNtcTime();
private:
    BluetoothService& bleService;
};

#endif // COMMUNICATION_MANAGER_H
