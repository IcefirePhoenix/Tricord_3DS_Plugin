#include "CTRPluginFramework/Graphics/Render.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"

namespace CTRPluginFramework
{
    void Render::DisplayPluginInfo(void)
    {
        const char g_ctrpfText[] = "Tricord is powered by CTRPluginFramework.";
        const char g_copyrightText[] = "Copyright © The Pixellizer Group";
        static u32 g_textXpos[2] = { 0 };

        g_textXpos[0] = (320 - (int)Renderer::GetTextSize(g_ctrpfText)) / 2;
        g_textXpos[1] = (320 - (int)Renderer::GetTextSize(g_copyrightText)) / 2;

        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        int posY = 85;
        Renderer::DrawGameFontString("Tricord Build Information:", 40, posY, 300, Color::Gainsboro);
        Renderer::DrawLine(40, posY, 25 * 6, Color::Gainsboro);

        posY += 10;
        Renderer::DrawGameFontString(Utils::Format("Tricord Version: %s", TRICORD_BUILD_METADATA).c_str(), 40, posY, 300, Color::Gainsboro);
        Renderer::DrawGameFontString(Utils::Format("CTRPF Version: %s", CTRPF_BUILD_METADATA).c_str(), 40, posY, 300, Color::Gainsboro);
        Renderer::DrawGameFontString(Utils::Format("Compiled: %s", COMPILE_DATE).c_str(), 40, posY, 300, Color::Gainsboro);

        posY = 165;
        Renderer::DrawGameFontString(g_ctrpfText, g_textXpos[0], posY, 300, Color::Gainsboro);
        Renderer::DrawGameFontString(g_copyrightText, g_textXpos[1], posY, 300, Color::Gainsboro);
    }

    float Render::GetTextWidth(const std::string& text)
    {
        return Renderer::GetTextSize(text.c_str());
    }

    int Render::CreateRandomSet(const std::string& set)
    {
        int selectedID = 0;
        while (selectedID < 63)
        {
            if (_randomSets.find(selectedID) == _randomSets.end())
                break;
            selectedID++;
        }
        if (selectedID >= 63)
            return -1;

        Utils::ConvertUTF8ToUTF16(_randomSets[selectedID], set);

        return selectedID;
    }

    void Render::EraseRandomSet(int setID)
    {
        _randomSets.erase(setID);
    }

    std::string Render::PullRandomCharacter(int setID)
    {
        auto it = _randomSets.find(setID);
        if (it == _randomSets.end())
            return "";
        string16& choices = it->second;
        if (choices.empty())
            return "";
        u16 newStr[2];
        newStr[0] = choices[Utils::Random(0, choices.size() - 1)];
        newStr[1] = '\0';
        std::string ret;
        Utils::ConvertUTF16ToUTF8(ret, newStr);
        return ret;
    }

    void Render::Interface::DrawPixel(int posX, int posY, const Color &color) {
        Renderer::DrawPixel(posX, posY, color);
    }

    void Render::Interface::ReadPixel(int posX, int posY, Color &outColor) {
        Renderer::ReadPixel(posX, posY, outColor);
    }

    void Render::Interface::DrawHLine(int posX, int posY, int length, const Color &color, int thickness, bool stippled) {
        if (stippled)
            Renderer::DrawStippledLine(posX, posY, length, color, thickness);
        else
            Renderer::DrawLine(posX, posY, length, color, thickness);
    }

    void Render::Interface::DrawVLine(int posX, int posY, int length, const Color &color, int thickness, bool stippled) {
        if (stippled)
            Renderer::DrawStippledLine(posX, posY, thickness, color, length);
        else
            Renderer::DrawLine(posX, posY, thickness, color, length);
    }

    void Render::Interface::DrawLine(const IntVector &start, const IntVector &end, const Color &color) {
        Renderer::DrawLine(start, end, color);
    }

    void Render::Interface::DrawRect(const IntRect& rect, const Color& color, bool fill, int thickness) {
        Renderer::DrawRect(rect.leftTop.x, rect.leftTop.y, rect.size.x, rect.size.y, color, fill, thickness);
    }

    void Render::Interface::DrawGameFontString(const std::string& str, int posX, int posY, Color color, int xLimit, int yLimit, bool autoWrap) {
        if (autoWrap)
            Renderer::DrawGameFontStringReturn(reinterpret_cast<const unsigned char*>(str.c_str()), posX, posY, xLimit, color, yLimit);
        else
            Renderer::DrawGameFontString(str.c_str(), posX, posY, xLimit, color);
    }

    void Render::Interface::DrawCustomIcon(const CustomIcon& icon, int posX, int posY) {
        Icon::DrawCustomIcon(icon, posX, posY);
    }
 }
