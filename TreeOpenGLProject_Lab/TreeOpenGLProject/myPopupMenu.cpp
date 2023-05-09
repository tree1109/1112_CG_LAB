#include <iostream>
#include "GL\freeglut.h"
#include "myPopupMenu.h"

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
    case OPT_1:
        MessagePrinter("wow");
        break;
    case OPT_2:
        MessagePrinter("cat is cutest");
        break;
    case OPT_3:
        MessagePrinter("bocchi kawaii~~");
        break;
    default:
        break;
    }
}

void myPopupMenu::GridDimensionsSubMenu(int option)
{
    switch (option)
    {
    case DIM_5:
        // TODO: set grid dim
        MessagePrinter("Set grid dimensions to 5");
        break;
    case DIM_10:
        // TODO: set grid dim
        MessagePrinter("Set grid dimensions to 10");
        break;
    case DIM_20:
        // TODO: set grid dim
        MessagePrinter("Set grid dimensions to 20");
        break;
    case DIM_50:
        // TODO: set grid dim
        MessagePrinter("Set grid dimensions to 50");
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myPopupMenu::ConfigMenu(void)
{
    // load grid dimensionsSubMenu sub menu
    int gridDimensionsSubMenu = glutCreateMenu(GridDimensionsSubMenu);
    glutAddMenuEntry("5", DIM_5);
    glutAddMenuEntry("10", DIM_10);
    glutAddMenuEntry("20", DIM_20);
    glutAddMenuEntry("50", DIM_50);

    // main menu
    int mainMenu = glutCreateMenu(MainMenu);
    glutAddSubMenu("Set Grid Dims. To", gridDimensionsSubMenu);
    glutAddMenuEntry("wow", OPT_1);
    glutAddMenuEntry("uwu", OPT_2);
    glutAddMenuEntry(">w<", OPT_3);
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "[info] \033[32m" << message << "\033[0m." << std::endl;
}
