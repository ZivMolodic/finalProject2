#pragma once
#include "Controller.h"
#include "MainMenu.h"

//game loop: closes only when user pressed exit
void Controller::run()
{
    auto mainMenu = MainMenu();
    mainMenu.menuLoop(WINDOW_SIZE);
}



