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
			makeMove(move);
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
Game::makeMove(GameMove direction)
{
	int32 toP, P;
	int32 fromS, toS, S;
	int32 moveS;
	int32 *x, *y;

	switch (direction)
	{
		case Left:
			toP = fSizeY;
			fromS = 0;
			moveS = 1;
			toS = fSizeX;
			x = &S;
			y = &P;
			break;
		case Right:
			toP = fSizeY;
			fromS = fSizeX - 1;
			moveS = -1;
			toS = -1;
			x = &S;
			y = &P;
			break;
		case Up:
			toP = fSizeX;
			fromS = 0;
			moveS = 1;
			toS = fSizeY;
			x = &P;
			y = &S;
			break;
		case Down:
			toP = fSizeX;
			fromS = fSizeY - 1;
			moveS = -1;
			toS = -1;
			x = &P;
			y = &S;
			break;
	}

	bool somethingChanged = false;

	for (int32 itP = 0; itP != toP; itP++)
	{
		int32 stopAt = fromS - moveS;
		for (int32 itS = fromS; itS != toS; itS += moveS)
		{
			int32 backS = itS;
			P = itP;
			S = itS;
			uint32 *source = boardAt(*x, *y);
			if (*source == 0)
				continue;
			bool merged = false;
			while (true)
			{
				if (backS == fromS || backS - 1 == stopAt)
					break;

				S = backS - moveS;
				uint32 *other = boardAt(*x, *y);
				if (*other == 0)
				{
					backS -= moveS;
					continue;
				}
				else if (*other == *source)
				{
					merged = true;
					backS -= moveS;
					break;
				}
				else
				{
					break;
				}
			}
			if (itS != backS)
			{
				somethingChanged = true;
				S = backS;
				uint32 *dest = boardAt(*x, *y);
				*dest = *source;
				if (merged)
					(*dest)++;
				*source = 0;
			}
		}
	}

	if (!somethingChanged)
	{
		// Consider notifying the sender of the move message that the move was
		// a nop.
		return;
	}

	bool found = false;
	uint32 placeX, placeY;
	while (!found)
	{
		placeX = std::rand() % fSizeX;
		placeY = std::rand() % fSizeY;

		found = BoardAt(placeX, placeY) == 0;
	}

	*boardAt(placeX, placeY) = std::rand() % 2 + 1;

	BMessage moved(H2048_MOVE_MADE);
	broadcastMessage(moved);
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
