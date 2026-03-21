#include "communication_protocol.h"

namespace CommunicationProtocol {

IncomingAction incomingActionFromString(const String& actionStr) {
  if (actionStr == "post-configuration-schedule") {
    return IncomingAction::POST_CONFIGURATION_SCHEDULE;
  }
  if (actionStr == "get-configuration-schedule") {
    return IncomingAction::GET_CONFIGURATION_SCHEDULE;
  }
  return IncomingAction::UNKNOWN;
}

String outgoingActionToString(OutgoingAction action) {
  switch (action) {
    case OutgoingAction::GET_CONFIGURATION_SCHEDULE:
      return "get-configuration-schedule";
    case OutgoingAction::POST_MEDICAMENTS_TAKEN_CONFIRMATION:
      return "medicaments-taken-confirmation";
    case OutgoingAction::POST_HEARTBEAT:
      return "heartbeat";
    default:
      return "unknown";
  }
}

String serializeOutgoingGetConfigurationSchedule(const MedicationConfiguration& config) {
  JsonDocument doc;
  doc["action"] = outgoingActionToString(OutgoingAction::GET_CONFIGURATION_SCHEDULE);
  JsonObject body = doc["body"].to<JsonObject>();
  JsonObject morning = body["morning"].to<JsonObject>();
  morning["interval_alert_trigger_minutes"] = config.morning.interval_notify_minutes;
  morning["alert"] = String(config.morning.alert.hour) + ":" + String(config.morning.alert.minute);
  JsonObject evening = body["evening"].to<JsonObject>();
  evening["interval_alert_trigger_minutes"] = config.evening.interval_notify_minutes;
  evening["alert"] = String(config.evening.alert.hour) + ":" + String(config.evening.alert.minute);
  String output;
  serializeJson(doc, output);
  return output;
}

String serializeOutgoingPostMedicamentsTaken(const String& taken_at) {
  JsonDocument doc;
  doc["action"] = outgoingActionToString(OutgoingAction::POST_MEDICAMENTS_TAKEN_CONFIRMATION);
  JsonObject body = doc["body"].to<JsonObject>();
  body["taken_at"] = taken_at;
  String output;
  serializeJson(doc, output);
  return output;
}

String serializeOutgoingPostHeartbeat() {
  JsonDocument doc;
  doc["action"] = outgoingActionToString(OutgoingAction::POST_HEARTBEAT);
  String output;
  serializeJson(doc, output);
  return output;
}

String serializeOutgoingGetConfigurationSchedule() {
  JsonDocument doc;
  doc["action"] = outgoingActionToString(OutgoingAction::GET_CONFIGURATION_SCHEDULE);
  String output;
  serializeJson(doc, output);
  return output;
}

MedicationConfiguration deserializeIncomingPostConfigurationSchedule(const JsonDocument& doc) {
  MedicationConfiguration config;

  JsonObjectConst body = doc["body"].as<JsonObjectConst>();
  JsonObjectConst morning = body["morning"].as<JsonObjectConst>();
  config.morning.interval_notify_minutes = morning["interval_alert_trigger_minutes"];
  String morning_alert = morning["alert"].as<const char*>();
  config.morning.alert.hour = morning_alert.substring(0, morning_alert.indexOf(':')).toInt();
  config.morning.alert.minute = morning_alert.substring(morning_alert.indexOf(':') + 1).toInt();

  JsonObjectConst evening = body["evening"].as<JsonObjectConst>();
  config.evening.interval_notify_minutes = evening["interval_alert_trigger_minutes"];
  String evening_alert = evening["alert"].as<const char*>();
  config.evening.alert.hour = evening_alert.substring(0, evening_alert.indexOf(':')).toInt();
  config.evening.alert.minute = evening_alert.substring(evening_alert.indexOf(':') + 1).toInt();

  return config;
}

} // namespace CommunicationProtocol