#pragma once
#include <string>

class myPopupMenu
{
public:
    static void CreatePopupMenu(void);
    static void MainMenu(int option);
    static void LoadObjectSubMenu(int option);
    static void RenderModeSubMenu(int option);
    static void ColorModeSubMenu(int option);

private:
    enum MainMenuOpt {
        OPT_1, 
        OPT_2, 
        OPT_3, 
        OPT_4
    };
    enum LoadObjectOpt {
        IMPORT_OBJ,
        OBJ_TEAPOT,
        OBJ_TEDDY,
        OBJ_OCTAHEDRON,
        OBJ_GOURD
    };
    enum RenderModeOpt {
        POINT, 
        LINE, 
        FACE
    };
    enum ColorModeOpt {
        SINGLE,
        RANDOM
    };

    static void ConfigMenu(void);
    static void MessagePrinter(std::string message);
};
