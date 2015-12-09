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
	int32 toP; // The size orthogonal to sliding
	int32 P; // The current position orthogonoal to sliding
	int32 fromS; // The index the tiles slide to
	int32 toS; // The farthest away index
	int32 S; // The current position parallel to sliding
	int32 moveS; // Reverse sliding direction
	int32 *x; // Pointer to the direction that corresponds to the X axis
	int32 *y; // Pointer to the direction that corresponds to the Y axis

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

	// The following code is completely agnostic of the direction the tiles are
	// acutally sliding

	// For each row of tiles that has to slide
	for (int32 itP = 0; itP != toP; itP++)
	{
		// For each tile that mey slide, in ascending length of possible slide
		for (int32 itS = fromS + moveS; itS != toS; itS += moveS)
		{
			// The tile which is sliding
			P = itP;
			S = itS;
			uint32 *source = boardAt(*x, *y);

			// Not a tile, nothing to slide
			if (*source == 0)
				continue;

			// Slide until hitting the wall or another tile
			int32 backS = itS - moveS;
			S = backS;
			while (backS != fromS && BoardAt(*x, *y) == 0)
			{
				backS -= moveS;
				S = backS;
			}

			// Can we merge with the time we hit?
			uint32 *dest = boardAt(*x, *y);
			bool merged = *dest == *source;

			if (!merged && *dest != 0)
			{
				// Hit a different tile, back off
				backS += moveS;
			}

			// Didn't move at all
			if (backS == itS)
				continue;

			somethingChanged = true;

			// Replace the space we are occupying with the source tile
			S = backS;
			dest = boardAt(*x, *y);
			*dest = *source; // Does nothing when merging
			*source = 0;

			// If we merged, increase the exponent
			if (merged)
				(*dest)++;
		}
	}

	if (!somethingChanged)
	{
		// Consider notifying the sender of the move message that the move was
		// a nop.
		return;
	}

	// Emplace new tile
	bool found = false;
	uint32 placeX, placeY;
	while (!found)
	{
		placeX = std::rand() % fSizeX;
		placeY = std::rand() % fSizeY;

		found = BoardAt(placeX, placeY) == 0;
	}
	*boardAt(placeX, placeY) = std::rand() % 2 + 1;

	// Notify boards what happened
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
