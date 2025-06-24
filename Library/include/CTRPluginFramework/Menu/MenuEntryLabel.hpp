#ifndef CTRPLUGINFRAMEWORK_MENUENTRYLABEL_HPP
#define CTRPLUGINFRAMEWORK_MENUENTRYLABEL_HPP

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include <string>
#include <memory>

namespace CTRPluginFramework
{
    class MenuEntryImpl;
    class MenuEntryLabel : public MenuItem
    {
    public:

        explicit MenuEntryLabel(std::string label);
        explicit MenuEntryLabel(void);
        ~MenuEntryLabel();

        /**
        * \brief Hide the entry from the menu. The entry will also be disabled
        */
        void Hide(void) const;

        /**
        * \brief Unhide an entry previously hidden
        */
        void Show(void) const;

        /**
        * \brief Check if this entry is visible in the menu
        * \return true if the entry is visible, false if the entry is hidden
        */
        bool IsVisible(void) const;

    private:
        friend class MenuFolder;
        friend class PluginMenu;
        std::unique_ptr<MenuEntryImpl>  _item;
    };
}

#endif
