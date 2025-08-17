#include <locale>

namespace Locale
{
    void Init();
    std::locale const& GetGlobalLocale();
    std::locale const& GetCalendarLocale();
}