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
    case MainMenuOpt::OPT_1:

        break;
    case MainMenuOpt::OPT_2:
        MessagePrinter("cat is cutest");
        break;
    case MainMenuOpt::OPT_3:
        MessagePrinter("bocchi kawaii~~");
        break;
    default:
        break;
    }
}

void myPopupMenu::ConfigMenu(void)
{
    // main menu
    int mainMenu = glutCreateMenu(MainMenu);
    glutAddMenuEntry("wakuwaku", MainMenuOpt::OPT_1);
    glutAddMenuEntry("uwu", MainMenuOpt::OPT_2);
    glutAddMenuEntry(">w<", MainMenuOpt::OPT_3);
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "[info] \033[32m" << message << "\033[0m." << std::endl;
}

