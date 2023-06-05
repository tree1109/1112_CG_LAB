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
    case OPT_1:
        switchDebugMode();
        break;
    case OPT_2:
        drawEdges();
        break;
    case OPT_3:
        drawPolygon();
        break;
    case OPT_4:
        switchCrowbarMode();
        break;
    case OPT_5:
        MessagePrinter("cat is cutest");
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
        setGridDimension(5);
        MessagePrinter("Set grid dimensions to 5");
        break;
    case DIM_10:
        setGridDimension(10);
        MessagePrinter("Set grid dimensions to 10");
        break;
    case DIM_20:
        setGridDimension(20);
        MessagePrinter("Set grid dimensions to 20");
        break;
    case DIM_50:
        setGridDimension(50);
        MessagePrinter("Set grid dimensions to 50");
        break;
    default:
        break;
    }
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
    glutAddMenuEntry("Normal / Debug Mode", OPT_1);
    glutAddMenuEntry("Draw edges", OPT_2);
    glutAddMenuEntry("Draw polygon", OPT_3);
    glutAddMenuEntry("Crowbar Mode", OPT_4);
    glutAddMenuEntry(">w<", OPT_5);
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "[info] \033[32m" << message << "\033[0m" << std::endl;
}
