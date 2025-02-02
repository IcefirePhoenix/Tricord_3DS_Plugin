#ifndef DESCRIPTIONUTILS_HPP
#define DESCRIPTIONUTILS_HPP

namespace CTRPluginFramework
{
    class DescUtils
    {
    public:
        static std::map<std::string, std::string> descriptions;

        static bool InitDescriptions(void);
        static std::string getDesc(std::string descTitle);
        static std::string getModeStr(int mode);
        static std::string getClientStr(bool clientOnly, int mode);
    };
}
#endif