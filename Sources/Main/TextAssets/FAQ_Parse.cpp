#include "FAQ_Content.json.h"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
    bool FAQ_Parse::GetContent(void)
    {
        std::string jsonString(reinterpret_cast<char *>(FAQ_Content_json), FAQ_Content_json_len);
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        if (jsonData.empty())
        {
            OSD::Notify("[ERROR] FAQ content cannot be loaded.", Color::Red);
            return false;
        }

        for (const auto &entry : jsonData["entries"])
        {
            std::string title = entry["title"];
            std::string combinedContent;

            for (const auto &section : entry["content"])
            {
                std::string header = section["section_header"];
                std::string body = section["body"];
                std::string optionalSeparator = title == "FAQ" ? "\n-----\n" : "";

                if (!header.empty())
                    combinedContent += header + "\n";

                combinedContent += body + "\n" + optionalSeparator + "\n";
            }

            GuideReader::AddTextEntry(title, combinedContent);
        }
        return true;
    }
}