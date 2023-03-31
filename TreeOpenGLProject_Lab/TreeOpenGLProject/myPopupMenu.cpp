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
    case MENU_OPTION::UWU:
        MessagePrinter("UwU is cutest");
        break;
    case MENU_OPTION::BOCCHI:
        MessagePrinter("Bocchi chan is say hello to you~");
        break;
    default:
        break;
    }
}

int myPopupMenu::BuildPopupMenu(void)
{
    int myMenu = glutCreateMenu(PopupMenu);
    glutAddMenuEntry("OwO", MENU_OPTION::UWU);
    glutAddMenuEntry("Bocchi is kawaii", MENU_OPTION::BOCCHI);
    return myMenu;
}

void myPopupMenu::MessagePrinter(std::string message)
{
    std::cout << "\033[3;0H\033[0K";
    std::cout << "[info] : \033[32m" << message << "\033[0m" << std::endl;
}

