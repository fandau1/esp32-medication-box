#ifndef PILLS_TRACKER
#define PILLS_TRACKER

enum class DayMedicationState {
    BEFORE_ALARM,
    AT_ALARM,
    AFTER_ALARM,
    MEDICATION_COLLECTED
};

class PillsTracker {
    public:
        void onPillTaken();

    private:
        DayMedicationState morningState;
        DayMedicationState eveningState;
};

#endif // PILLS_TRACKER