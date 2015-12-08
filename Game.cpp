/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#include "Game.h"
#include "GameBoard.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <Message.h>
#include <Messenger.h>

Game::Game(uint32 sizeX, uint32 sizeY)
	:
	fSizeX(sizeX),
	fSizeY(sizeY),
	fTargets(),
	fInGame(false)
{
	fBoard = new uint32[fSizeX * fSizeY];
	Run();
}

Game::~Game()
{
	delete [] fBoard;
}

void
Game::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		GameMove move;
		case H2048_MAKE_MOVE:
			move = (GameMove)message->FindInt32("direction");
			break;
		default:
			break;
	}
}

void
Game::StartWatching(BMessenger *target)
{
	fTargets.push_back(target);
}

bool
Game::StopWatching(BMessenger *target)
{
	return fTargets.erase(std::remove(fTargets.begin(), fTargets.end(), target));
}

uint32
Game::BoardAt(uint32 x, uint32 y) const
{
	return fBoard[x * fSizeY + y];
}

void
Game::NewGame()
{
	// Clear the board
	for (uint32 x = 0; x < fSizeX; x++)
	{
		for (uint32 y = 0; y < fSizeY; y++)
		{
			*boardAt(x, y) = 0;
		}
	}

	// Place the starting tiles
	uint32 placed = 0;
	while (placed < 2)
	{
		// This isn't perfectly distributed, but since, in general, RAND_MAX
		// is *much* larger than fSizeX/Y, it does not matter
		uint32 x = std::rand() % fSizeX;
		uint32 y = std::rand() % fSizeY;

		uint32 *pos = boardAt(x, y);

		// Already placed that tile
		if (*pos != 0)
			continue;

		// Either 2^1 or 2^2
		*pos = std::rand() % 2 + 1;
		placed++;
	}

	// Notify game boards that a game has started
	BMessage startNotification(H2048_GAME_STARTED);
	broadcastMessage(startNotification);

	fInGame = true;
}

uint32
Game::SizeX() const
{
	return fSizeX;
}

uint32
Game::SizeY() const
{
	return fSizeY;
}

void
Game::broadcastMessage(BMessage &msg)
{
	std::vector<BMessenger *>::iterator it;
	for (it = fTargets.begin(); it != fTargets.end(); it++)
	{
		(*it)->SendMessage(&msg);
	}
}

uint32 *
Game::boardAt(uint32 x, uint32 y)
{
	return fBoard + (x * fSizeY + y);
}
