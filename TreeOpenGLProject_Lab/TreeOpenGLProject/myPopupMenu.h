#pragma once
#include <string>

class myPopupMenu
{
public:
    static void SetupPopupMenu(void);
    static void PopupMenu(int option);

private:
    enum MenuOption
    {
        OPT_1,
        OPT_2
    };

    static int BuildPopupMenu(void);
    static void MessagePrinter(std::string message);
};
