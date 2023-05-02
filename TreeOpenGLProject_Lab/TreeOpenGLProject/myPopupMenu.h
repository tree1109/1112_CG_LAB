#pragma once
#include <string>

class myPopupMenu
{
public:
    static void CreatePopupMenu(void);
    static void MainMenu(int option);

private:
    enum MainMenuOpt {
        OPT_1, 
        OPT_2, 
        OPT_3, 
        OPT_4
    };
    static void ConfigMenu(void);
    static void MessagePrinter(std::string message);
};
