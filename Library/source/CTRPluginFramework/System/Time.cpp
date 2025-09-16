#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#include <algorithm>
#include <ctime>
#include "types.h"

#include "CTRPluginFramework/System/Time.hpp"

namespace CTRPluginFramework
{
    const Time Time::Zero;

    float   Time::AsSeconds(void) const
    {
        return (_ticks / (float)TicksPerSecond);
    }


    int     Time::AsMilliseconds(void) const
    {
        return static_cast<int>(_ticks / (TicksPerSecond / 1000.f));
    }


    s64     Time::AsMicroseconds(void) const
    {
        return static_cast<s64>(_ticks / (TicksPerSecond / 1000000.f));
    }

    std::string Time::getCurrentTime(bool getDate)
    {
        std::time_t now = std::time(nullptr);
        std::tm tm = *std::localtime(&now);

        std::ostringstream out;
        out << std::put_time(&tm, getDate ? "%F" : "T%H%M%S");
        return out.str();
    }

    std::string Time::GetDate(void)
    {
        return getCurrentTime(true);
    }

    std::string Time::GetTime(void)
    {
        return getCurrentTime(false);
    }

    // std::get_time is broken in this env, manual parsing needed
    std::string Time::ParseDate(std::string date)
    {
        char buffer[30];
        int year, month, day;
        std::tm tm = {};

        if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) == 3)
        {
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;

            std::strftime(buffer, sizeof(buffer), "%b %d, %Y", &tm);
            return std::string(buffer);
        }
        else
            return "Unknown";
    }
}
