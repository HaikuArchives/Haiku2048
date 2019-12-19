/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#include <ctime>
#include <cstdlib>
#include <Application.h>
#include <Messenger.h>
#include "WindowBoard.h"
#include "Game.h"

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
	WindowBoard *win = new WindowBoard(theGame);
	BMessenger gameMsg(NULL, theGame);
	gameMsg.SendMessage(H2048_NEW_GAME);
}

int main()
{
	std::srand(std::time(NULL));
	App app;
	app.Run();
	return 0;
}
