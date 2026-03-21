#ifndef COMUNICATION_TYPES_H
#define COMUNICATION_TYPES_H

struct MedicationTime {
  int hour;
  int minute;
};

struct ScheduleSetting {
  MedicationTime alert;
  int interval_notify_minutes;
};

// Define the configuration structure to be stored in EEPROM
struct MedicationConfiguration {
  ScheduleSetting morning;
  ScheduleSetting evening;
};

#endif 