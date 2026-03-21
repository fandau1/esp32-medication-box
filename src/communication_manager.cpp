#include "communication_manager.h"
#include "communication_protocol.h"
#include <Arduino.h>
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(BluetoothService& bleService)
    : bleService(bleService) {}

void CommunicationManager::begin() {
    // The onBleReceive method needs to be called from a static context or a lambda without captures.
    // We can use a lambda to capture 'this' and forward the call.
    bleService.setOnReceive([this](const std::string& value) {
        this->onBleReceive(value);
    });
}

void CommunicationManager::loop() {
  static unsigned long lastHeartbeatAtMs = 0;
  constexpr unsigned long HEARTBEAT_INTERVAL_MS = 60000;

  if (!bleService.isConnected()) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastHeartbeatAtMs >= HEARTBEAT_INTERVAL_MS) {
    outgoingPostHearthbeat();
    lastHeartbeatAtMs = now;
  }
}

bool CommunicationManager::outgoingPostMedicamentsTakenConfirmation() {
  if (!bleService.isConnected()) {
    return false;
  }

  const String currentTimestamp = Time_manager::getUTCString();
  String response = CommunicationProtocol::serializeOutgoingPostMedicamentsTaken(String(currentTimestamp));
  bleService.sendJson(response);
  return true;
}

void CommunicationManager::outgoingPostHearthbeat() {
  if (!bleService.isConnected()) {
    return;
  }

  String response = CommunicationProtocol::serializeOutgoingPostHeartbeat();
  bleService.sendJson(response);
}

void CommunicationManager::outgoingGetNtcTime() {
  if (!bleService.isConnected()) {
    return;
  }

  const unsigned long currentTimestamp = millis() / 1000;
  String response = CommunicationProtocol::serializeOutgoingGetNtcTime(currentTimestamp);
  bleService.sendJson(response);
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
      EEPROMService::saveConfiguration(config);
      Serial.println("Configuration schedule saved to EEPROM");
      break;
    }
    case CommunicationProtocol::IncomingAction::GET_CONFIGURATION_SCHEDULE: {
      MedicationConfiguration config = EEPROMService::loadConfiguration();
      String response = CommunicationProtocol::serializeOutgoingGetConfigurationSchedule(config);
      bleService.sendJson(response);
      Serial.println("Sended requested configuration response");
      break;
    }
    case CommunicationProtocol::IncomingAction::POST_NTC_TIME: {
      unsigned long unix_timestamp = CommunicationProtocol::deserializeIncomingGetNtcTimeTimestamp(doc);
      Time_manager::setRTC(unix_timestamp);
      break;
    }
    default:
      Serial.println("Unknown action");
      break;
  }
}
