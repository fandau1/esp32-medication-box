#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <ArduinoJson.h>
#include "communication_types.h"

namespace CommunicationProtocol {

enum class IncomingAction {
  UNKNOWN,
  GET_CONFIGURATION_SCHEDULE,
  UPLOAD_CONFIGURATION_SCHEDULE,
};

enum class OutgoingAction {
  UNKNOWN,
  GET_CONFIGURATION_SCHEDULE,
  MEDICAMENTS_TAKEN_CONFIRMATION,
  HEARTBEAT
};

IncomingAction incomingActionFromString(const String& actionStr);
String outgoingActionToString(OutgoingAction action);

String serializeConfiguration(const MedicationConfiguration& config);
String serializeMedicamentsTaken(const String& taken_at);
String serializeHeartbeat();
MedicationConfiguration deserializeConfiguration(const JsonDocument& doc);

} // namespace CommunicationProtocol

#endif // COMMUNICATION_PROTOCOL_H