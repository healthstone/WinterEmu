#include "Util.hpp"

tm Util::TimeBreakdown(time_t time) {
    tm timeLocal;
    localtime_r(&time, &timeLocal);
    return timeLocal;
}