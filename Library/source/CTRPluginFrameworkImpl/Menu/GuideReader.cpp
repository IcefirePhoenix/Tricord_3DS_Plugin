#include "CTRPluginFrameworkImpl/Menu/GuideReader.hpp"

namespace CTRPluginFramework
{
    MenuFolderImpl *mFolder = new MenuFolderImpl("FAQ and Credits");
    std::map<std::string, std::string> contentMap;

    void GuideReader::AddTextEntry(const std::string& entryName, const std::string& content)
    {
        contentMap[entryName] = content;

        MenuEntryImpl *entry = new MenuEntryImpl(entryName);
        mFolder->Append(entry);
    }

    GuideReader::GuideReader(void):
    _isOpen(false), _menu(mFolder, Icon::DrawManual), _guideTB("", "", Window::TopWindow.GetRect()), _text(""), _last(nullptr)
    {
    }

    bool GuideReader::operator()(EventList &eventList, Time &delta)
    {
        _isOpen = true;

        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        _Update(delta);

        Draw();
        return (Window::BottomWindow.MustClose() || !_isOpen);
    }

    bool GuideReader::Draw(void)
    {
        if (!_isOpen)
            return (false);

        Renderer::SetTarget(TOP);

        if (_guideTB.IsOpen())
            _guideTB.Draw();
        else
            _menu.Draw();

        Render::DisplayPluginInfo();
        return true;
    }

    void GuideReader::_Update(Time delta)
    {
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouched, touchPos);
    }

    bool GuideReader::_ProcessEvent(Event &event)
    {
        if (!_isOpen)
            return (false);

        if (_guideTB.IsOpen())
            _guideTB.ProcessEvent(event);
        else
        {
            MenuItem *item = nullptr;

            int ret = _menu.ProcessEvent(event, &item);
            if (ret == MenuEvent::MenuClose)
            {
                Close();
                return false;
            }

            else if (ret == MenuEvent::EntrySelected)
            {
                MenuEntryImpl *entry = (MenuEntryImpl *)item;
                if (entry != _last)
                {
                    _last = entry;

                    auto it = contentMap.find(entry->name);

                    // shouldn't run into this, but just in case...
                    if (it == contentMap.end())
                        return false;

                    const std::string& content = it->second;

                    _text.clear();
                    _text.reserve(content.size());
                    _text = content;

                    _guideTB.Update(entry->name, _text);
                    _guideTB.Open();
                }
                else
                {
                    _guideTB.Open();
                }
            }
        }
        return true;
    }

    void    GuideReader::Open(void)
    {
        _isOpen = true;
    }

    void    GuideReader::Close(void)
    {
        _isOpen = false;
    }
}