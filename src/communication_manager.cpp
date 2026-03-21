#include "communication_manager.h"
#include "communication_protocol.h"
#include <Arduino.h>
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(BluetoothService& bleService, EEPROMService& eepromService)
    : bleService(bleService), eepromService(eepromService) {}

void CommunicationManager::begin() {
    // The onBleReceive method needs to be called from a static context or a lambda without captures.
    // We can use a lambda to capture 'this' and forward the call.
    bleService.setOnReceive([this](const std::string& value) {
        this->onBleReceive(value);
    });
}

void CommunicationManager::onBleReceive(const std::string& value) {
  Serial.println("Received: " + String(value.c_str()));
  JsonDocument doc;
  deserializeJson(doc, value);
  String action = doc["action"];

  CommunicationProtocol::IncomingAction incomingAction = CommunicationProtocol::incomingActionFromString(action);

  switch (incomingAction) {
    case CommunicationProtocol::IncomingAction::POST_CONFIGURATION_SCHEDULE: {
      MedicationConfiguration config = CommunicationProtocol::deserializeIncomingPostConfigurationSchedule(doc);
      eepromService.saveConfiguration(config);
      Serial.println("Configuration schedule saved to EEPROM");
      break;
    }
    case CommunicationProtocol::IncomingAction::GET_CONFIGURATION_SCHEDULE: {
      MedicationConfiguration config = eepromService.loadConfiguration();
      String response = CommunicationProtocol::serializeOutgoingGetConfigurationSchedule(config);
      bleService.sendJson(response);
      Serial.println("Sended requested configuration response");
      break;
    }
    default:
      Serial.println("Unknown action");
      break;
  }
}
