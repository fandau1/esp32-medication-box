// Define the schedule enum
enum MedicationSchedule {
  SUNDAY_MORNING,
  SUNDAY_EVENING,
  MONDAY_MORNING,
  MONDAY_EVENING,
  TUESDAY_MORNING,
  TUESDAY_EVENING,
  WEDNESDAY_MORNING,
  WEDNESDAY_EVENING,
  THURSDAY_MORNING,
  THURSDAY_EVENING,
  FRIDAY_MORNING,
  FRIDAY_EVENING,
  SATURDAY_MORNING,
  SATURDAY_EVENING,
  SCHEDULE_COUNT // To get the size of the enum
};

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