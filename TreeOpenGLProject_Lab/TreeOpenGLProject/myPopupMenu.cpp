#include "myPopupMenu.h"
#include "GL\freeglut.h"
#include <iostream>

void myPopupMenu::SetupPopupMenu(void)
{
    // create a menu and attach it to the right button
    BuildPopupMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myPopupMenu::PopupMenu(int option)
{
    // erase the previous text
    std::cout << "\033[3;0H\033[0K";

    switch (option)
    {
    case MENU_OPTION::OPT_1:
        MessagePrinter("cat is cutest");
        break;
    case MENU_OPTION::OPT_2:
        MessagePrinter("bocchi");
        break;
    default:
        break;
    }
}

int myPopupMenu::BuildPopupMenu(void)
{
    int myMenu = glutCreateMenu(PopupMenu);
    glutAddMenuEntry("owo", MENU_OPTION::OPT_1);
    glutAddMenuEntry("bocchi", MENU_OPTION::OPT_2);
    return myMenu;
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "\033[5;0H\033[0K";
    std::cout << "[info] : \033[32m" << message << "\033[0m" << std::endl;
}

