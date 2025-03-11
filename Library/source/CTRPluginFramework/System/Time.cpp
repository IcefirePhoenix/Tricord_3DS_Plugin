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

    std::string Time::GetDate(bool addTime)
    {
        char buffer[20] = { 0 };
        time_t t = time(NULL);
        struct tm *timeinfo = localtime(&t);

        strftime(buffer, sizeof(buffer), addTime ? "%F_%H.%M" : "%F", timeinfo); // colons not allowed -> auto-truncates string as a side effect

        std::string dateStr(buffer);

        std::replace(dateStr.begin(), dateStr.end(), '/', '.');
        return dateStr;
    }

    std::string Time::ParseDate(std::string date)
    {
        std::map<std::string, std::string> monthMap = {
            {"01", "Jan"}, {"02", "Feb"}, {"03", "Mar"}, {"04", "Apr"},
            {"05", "May"}, {"06", "Jun"}, {"07", "Jul"}, {"08", "Aug"},
            {"09", "Sep"}, {"10", "Oct"}, {"11", "Nov"}, {"12", "Dec"}
        };

        std::istringstream stream(date);
        std::string year, month, day;

        std::getline(stream, year, '-');
        std::getline(stream, month, '-');
        std::getline(stream, day);

        if (monthMap.find(month) == monthMap.end())
            return "Invalid Date";

        std::stringstream formattedDate;
        formattedDate << monthMap[month] << " " << std::stoi(day) << ", " << year;

        return formattedDate.str();
    }
}
