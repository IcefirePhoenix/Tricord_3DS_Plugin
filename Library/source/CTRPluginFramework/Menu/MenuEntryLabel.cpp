#include "types.h"
#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFramework/Menu/MenuEntryLabel.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"

namespace CTRPluginFramework
{
    MenuEntryLabel::MenuEntryLabel(std::string label) :
        MenuItem(MenuType::Entry), _item(new MenuEntryImpl(label, ""))
    {
        _item->_flags.restricted = true;
        _item->_flags.isUnselectable = true;
        _item->Flags.useSeparatorSpacer = false;
        _item->SetAltIcon(true);
    }

    MenuEntryLabel::MenuEntryLabel(void) :
        MenuItem(MenuType::Entry),  _item(new MenuEntryImpl(" ", ""))
    {
        _item->_flags.restricted = true;
        _item->_flags.isUnselectable = true;
        _item->Flags.useSeparatorSpacer = true;
        _item->SetAltIcon(true);
    }

    MenuEntryLabel::~MenuEntryLabel()
    {
    }

    void MenuEntryLabel::Hide(void) const
    {
        _item->Hide();
    }

    void MenuEntryLabel::Show(void) const
    {
        _item->Show();
    }

    bool MenuEntryLabel::IsVisible() const
    {
        return (_item->IsVisible());
    }

    void MenuEntryLabel::SetName(std::string newLabel)
    {
        _item->name = newLabel;
    }
}
