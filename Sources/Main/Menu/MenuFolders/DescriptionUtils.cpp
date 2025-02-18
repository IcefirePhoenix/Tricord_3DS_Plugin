#include "EntryDescList.json.h"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
    std::map<std::string, std::string> DescUtils::descriptions;

    /* ------------------ */

    std::string DescUtils::getModeStr(int mode)
    {
        switch (mode)
        {
            case 0:
                return "\n\nCan only be used in single player.";
            case 1:
                return "\n\nCan be used in single player OR multiplayer.";
            case 2:
                return "\n\nCan be used in single player OR multiplayer if all players enable this entry.";
            case 3:
                return "\n\nCan be used in single player OR multiplayer if all players apply the same selection(s) or value(s).";
            case 4:
                return "\n\nCan only be used in multiplayer.";
            default:
                return "";
        }
    }

    std::string DescUtils::getClientStr(bool clientOnly, int mode)
    {
        if (clientOnly)
            return "This entry is only observable client-side.";
        else
        {
            if (mode >= 1 && mode <= 4)
                return "\n\nThis entry is observable across all connected clients.";
        }
        return ""; // single-player; doesn't apply
    }

    std::string DescUtils::getDesc(std::string descTitle)
    {
        std::string descStr = DescUtils::descriptions[descTitle];

        if (descStr.empty())
            OSD::Notify("[ERROR] Entry description cannot be found for " << descTitle << ".", Color::Red);

        return descStr;
    }

    bool DescUtils::InitDescriptions(void)
    {
        std::string jsonString(reinterpret_cast<char *>(EntryDescList_json), EntryDescList_json_len);
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        if (jsonData.empty())
        {
            OSD::Notify("ERROR: Entry descriptions cannot be loaded.", Color::Red);
            return false;
        }

        for (const auto &entry : jsonData["descriptions"])
        {
            std::string name = entry["name"];
            const auto &content = entry["content"];

            std::string intro = content["intro"].get<std::string>();
            std::string note = content["note"].get<std::string>();

            int modeCompatibility = content["modeCompatibility"].get<int>();
            bool clientOnly = content["clientOnly"].get<bool>();

            if (!note.empty())
                intro += "\n\n";

            std::string finalDesc = intro + note + getModeStr(modeCompatibility) + getClientStr(clientOnly, modeCompatibility);

            DescUtils::descriptions[name] = finalDesc;
        }
        return true;
    }
}