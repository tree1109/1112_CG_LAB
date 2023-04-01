#pragma once
#include <string>

class myPopupMenu
{
public:
    typedef enum MenuOption
    {
        OPT_1,
        OPT_2
    } MENU_OPTION;

    void SetupPopupMenu(void);
    static void PopupMenu(int);

private:
    static int BuildPopupMenu(void);
    static void MessagePrinter(std::string);
};
