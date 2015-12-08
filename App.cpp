/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#include <ctime>
#include <cstdlib>
#include <Application.h>
#include "MainWindow.h"
#include "Game.h"
#include "TerminalBoard.h"

class App : public BApplication
{
public:
			App();
};

App::App()
	:
	BApplication("application/x-vnd.Haiku-Haiku2048")
{
	Game *theGame = new Game(4, 4);
	TerminalBoard *board = new TerminalBoard(theGame);
	theGame->NewGame();
}

int main()
{
	std::srand(std::time(NULL));
	App app;
	app.Run();
	return 0;
}
