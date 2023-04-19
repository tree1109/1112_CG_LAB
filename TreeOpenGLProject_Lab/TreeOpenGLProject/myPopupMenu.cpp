#include "myPopupMenu.h"
#include "GL\freeglut.h"
#include <iostream>

void myPopupMenu::CreatePopupMenu(void)
{
    // create a menu and attach it to the right button
    ConfigMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myPopupMenu::MainMenu(int option)
{
    switch (option)
    {
    case MainMenuOpt::OPT_1:
        MessagePrinter("cat is cutest");
        break;
    case MainMenuOpt::OPT_2:
        MessagePrinter("bocchi kawaii~~");
        break;
    default:
        break;
    }
}

void myPopupMenu::LoadObjectMenu(int option)
{
    switch (option)
    {
    case LoadObjectOpt::IMPORT_OBJ:
        MessagePrinter("Import user OBJ");
        break;
    case LoadObjectOpt::OBJ_TEAPOT:
        MessagePrinter("Load object [Teapot]");
        break;
    case LoadObjectOpt::OBJ_TEDDY:
        MessagePrinter("Load object [Teddy]");
        break;
    case LoadObjectOpt::OBJ_OCTAHEDRON:
        MessagePrinter("Load object [Octahedron]");
        break;
    case LoadObjectOpt::OBJ_GOURD:
        MessagePrinter("Load object [Gourd]");
        break;
    default:
        break;
    }
}

void myPopupMenu::RenderModeMenu(int option)
{
    switch (option)
    {
    case RenderModeOpt::POINT:
        MessagePrinter("Set render mode to [Point]");
        break;
    case RenderModeOpt::LINE:
        MessagePrinter("Set render mode to [Line]");
        break;
    case RenderModeOpt::FACE:
        MessagePrinter("Set render mode to [Face]");
        break;
    default:
        break;
    }
}

void myPopupMenu::ColorModeMenu(int option)
{
    switch (option)
    {
    case ColorModeOpt::SINGLE:
        MessagePrinter("Set color mode to [Single]");
        break;
    case ColorModeOpt::RANDOM:
        MessagePrinter("Set color mode to [Random]");
        break;
    default:
        break;
    }
}

void myPopupMenu::ConfigMenu(void)
{
    // load object sub menu
    int loadObjectSubMenu = glutCreateMenu(LoadObjectMenu);
    glutAddMenuEntry("Import OBJ", LoadObjectOpt::IMPORT_OBJ);
    glutAddMenuEntry("Teapot", LoadObjectOpt::OBJ_TEAPOT);
    glutAddMenuEntry("Teddy", LoadObjectOpt::OBJ_TEDDY);
    glutAddMenuEntry("Octahedron", LoadObjectOpt::OBJ_OCTAHEDRON);
    glutAddMenuEntry("Gourd", LoadObjectOpt::OBJ_GOURD);

    // render mode sub menu
    int renderModeSubMenu = glutCreateMenu(RenderModeMenu);
    glutAddMenuEntry("Point", RenderModeOpt::POINT);
    glutAddMenuEntry("Line", RenderModeOpt::LINE);
    glutAddMenuEntry("Face", RenderModeOpt::FACE);

    // color mode sub menu
    int colorModeSubMenu = glutCreateMenu(ColorModeMenu);
    glutAddMenuEntry("Single", ColorModeOpt::SINGLE);
    glutAddMenuEntry("Random", ColorModeOpt::RANDOM);

    // main menu
    int mainMenu = glutCreateMenu(MainMenu);
    glutAddSubMenu("Load object", loadObjectSubMenu);
    glutAddSubMenu("Set render mode", renderModeSubMenu);
    glutAddSubMenu("Set color mode", colorModeSubMenu);
    glutAddMenuEntry("uwu", MainMenuOpt::OPT_1);
    glutAddMenuEntry(">w<", MainMenuOpt::OPT_2);
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "[info] : \033[32m" << message << "\033[0m." << std::endl;
}

