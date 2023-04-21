#include <iostream>
#include "GL\freeglut.h"
#include "myPopupMenu.h"
#include "main.h"

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

void myPopupMenu::LoadObjectSubMenu(int option)
{
    switch (option)
    {
    case LoadObjectOpt::IMPORT_OBJ:
        MessagePrinter("Import user OBJ");
        break;
    case LoadObjectOpt::OBJ_TEAPOT:
        setCurrentObject(OBJECT::TEAPOT);
        MessagePrinter("Load object [Teapot]");
        break;
    case LoadObjectOpt::OBJ_TEDDY:
        setCurrentObject(OBJECT::TEDDY);
        MessagePrinter("Load object [Teddy]");
        break;
    case LoadObjectOpt::OBJ_OCTAHEDRON:
        setCurrentObject(OBJECT::OCTAHEDRON);
        MessagePrinter("Load object [Octahedron]");
        break;
    case LoadObjectOpt::OBJ_GOURD:
        setCurrentObject(OBJECT::GOURD);
        MessagePrinter("Load object [Gourd]");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myPopupMenu::RenderModeSubMenu(int option)
{
    switch (option)
    {
    case RenderModeOpt::POINT:
        currentRenderMode = RENDER_MODE::POINTS;
        MessagePrinter("Set render mode to [Point]");
        break;
    case RenderModeOpt::LINE:
        currentRenderMode = RENDER_MODE::LINES;
        MessagePrinter("Set render mode to [Line]");
        break;
    case RenderModeOpt::FACE:
        currentRenderMode = RENDER_MODE::FACES;
        MessagePrinter("Set render mode to [Face]");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myPopupMenu::ColorModeSubMenu(int option)
{
    switch (option)
    {
    case ColorModeOpt::SINGLE:
        currentColorMode = COLOR_MODE::SINGLE;
        MessagePrinter("Set color mode to [Single]");
        break;
    case ColorModeOpt::RANDOM:
        currentColorMode = COLOR_MODE::RANDOM;
        MessagePrinter("Set color mode to [Random]");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myPopupMenu::ConfigMenu(void)
{
    // load object sub menu
    int loadObjectSubMenu = glutCreateMenu(LoadObjectSubMenu);
    glutAddMenuEntry("Import OBJ", LoadObjectOpt::IMPORT_OBJ);
    glutAddMenuEntry("Teapot", LoadObjectOpt::OBJ_TEAPOT);
    glutAddMenuEntry("Teddy", LoadObjectOpt::OBJ_TEDDY);
    glutAddMenuEntry("Octahedron", LoadObjectOpt::OBJ_OCTAHEDRON);
    glutAddMenuEntry("Gourd", LoadObjectOpt::OBJ_GOURD);

    // render mode sub menu
    int renderModeSubMenu = glutCreateMenu(RenderModeSubMenu);
    glutAddMenuEntry("Point", RenderModeOpt::POINT);
    glutAddMenuEntry("Line", RenderModeOpt::LINE);
    glutAddMenuEntry("Face", RenderModeOpt::FACE);

    // color mode sub menu
    int colorModeSubMenu = glutCreateMenu(ColorModeSubMenu);
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

