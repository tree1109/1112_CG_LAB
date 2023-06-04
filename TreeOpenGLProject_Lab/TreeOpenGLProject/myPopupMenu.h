#pragma once
#include <string>

class myPopupMenu
{
public:
    static void CreatePopupMenu(void);
    static void MainMenu(int option);
    static void GridDimensionsSubMenu(int option);

private:
    enum MainMenuOpt {
        OPT_1, 
        OPT_2, 
        OPT_3,
        OPT_4
    };
    enum GridDimensionOpt {
        DIM_5,
        DIM_10,
        DIM_20,
        DIM_50
    };
    static void ConfigMenu(void);
    static void MessagePrinter(std::string message);
};
