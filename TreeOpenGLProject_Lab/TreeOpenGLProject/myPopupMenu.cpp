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
    switch (option)
    {
    case MenuOption::OPT_1:
        MessagePrinter("cat is cutest");
        break;
    case MenuOption::OPT_2:
        MessagePrinter("bocchi kawaii~~");
        break;
    default:
        break;
    }
}

int myPopupMenu::BuildPopupMenu(void)
{
    int myMenu = glutCreateMenu(PopupMenu);
    glutAddMenuEntry("owo", MenuOption::OPT_1);
    glutAddMenuEntry("bocchi", MenuOption::OPT_2);
    return myMenu;
}

void myPopupMenu::MessagePrinter(std::string message)
{
    // clear the previous text
    std::cout << "\033[5;0H\033[0K";
    std::cout << "[info] : \033[32m" << message << "\033[0m" << std::endl;
}

