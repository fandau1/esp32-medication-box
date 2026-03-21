#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <ArduinoJson.h>
#include "communication_types.h"

namespace CommunicationProtocol {

enum class IncomingAction {
  UNKNOWN,
  GET_CONFIGURATION_SCHEDULE,
  POST_CONFIGURATION_SCHEDULE,
};

enum class OutgoingAction {
  UNKNOWN,
  GET_CONFIGURATION_SCHEDULE,
  POST_MEDICAMENTS_TAKEN_CONFIRMATION,
  POST_HEARTBEAT
};

IncomingAction incomingActionFromString(const String& actionStr);
String outgoingActionToString(OutgoingAction action);

// ESP32 -> Phone
String serializeOutgoingGetConfigurationSchedule(const MedicationConfiguration& config);
String serializeOutgoingPostMedicamentsTaken(const String& taken_at);
String serializeOutgoingPostHeartbeat();

// Phone -> ESP32
String serializeIncomingGetConfigurationSchedule();
MedicationConfiguration deserializeIncomingPostConfigurationSchedule(const JsonDocument& doc);

} // namespace CommunicationProtocol

#endif // COMMUNICATION_PROTOCOL_H