#include "Helpers/TimeGetter.hpp"
#include "CommonParameters.hpp"

UTCTimestamp TimeGetter::getCurrentTimeUTC() {
    return getCurrentTimeDefaultCUC().toUTCtimestamp();
}

Time::DefaultCUC TimeGetter::getCurrentTimeDefaultCUC() {
    // ToDo: Fix this
    //    return CommonParameters::time.getValue();
    Time::DefaultCUC a = Time::DefaultCUC();        // Dummy
    return a;
}