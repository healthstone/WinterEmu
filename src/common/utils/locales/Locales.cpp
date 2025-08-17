#include "Locales.hpp"
#include <boost/locale/generator.hpp>

namespace {
    std::locale _global;
    std::locale _calendar;
}

void Locale::Init() {
    // Change global locale from "C" to UTF-8 for c runtime functions
    std::locale utf8("");
    _global = utf8;
    _global = std::locale(_global, std::locale::classic(), std::locale::numeric);
    std::locale::global(_global);

    std::setlocale(LC_ALL, "");
    std::setlocale(LC_NUMERIC, "C");

    boost::locale::generator g;
    _calendar = g.generate(utf8, "");
}

std::locale const &Locale::GetGlobalLocale() {
    return _global;
}

std::locale const &Locale::GetCalendarLocale() {
    return _calendar;
}
