/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "GameBoard.h"
#include "Game.h"

#include <Message.h>

GameBoard::GameBoard(Game *target)
	:
	fTarget(target),
	fToSelf(NULL, this)
{
	Run();
	fTarget->StartWatching(&fToSelf);
}

GameBoard::~GameBoard()
{
	fTarget->StopWatching(&fToSelf);
}

void
GameBoard::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case H2048_GAME_STARTED:
			gameStarted();
			break;
		case H2048_GAME_ENDED:
			gameEnded();
			break;
		case H2048_MOVE_MADE:
			moveMade();
			break;
		case H2048_REQUEST_NAME:
			nameRequest();
		default:
			break;
	}
}
