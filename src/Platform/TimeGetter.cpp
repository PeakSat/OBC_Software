#include "Helpers/TimeGetter.hpp"

UTCTimestamp TimeGetter::getCurrentTimeUTC() {
    return getCurrentTimeDefaultCUC().toUTCtimestamp();
}

Time::DefaultCUC TimeGetter::getCurrentTimeDefaultCUC() {
    return timeKeepingTask->getSavedTime();
}