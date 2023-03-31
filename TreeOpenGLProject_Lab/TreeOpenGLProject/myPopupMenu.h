#pragma once
#include <string>

class myPopupMenu
{
public:
    typedef enum MenuOption
    {
        UWU,
        BOCCHI
    } MENU_OPTION;

    void SetupPopupMenu(void);
    static void PopupMenu(int);

private:
    static int BuildPopupMenu(void);
    static void MessagePrinter(std::string);
};
