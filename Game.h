/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#ifndef GAME_H
#define GAME_H

#include <Looper.h>
#include <ObjectList.h>
#include <SupportDefs.h>

class BMessage;
class BMessenger;
class GameBoard;

class Game : public BLooper
{
public:
						Game(uint32 sizeX, uint32 sizeY);
						~Game();
			void		MessageReceived(BMessage *message);

			void		StartWatching(BMessenger *target);
			bool		StopWatching(BMessenger *target);

private:
			uint32 *	boardAt(uint32 x, uint32 y);

private:
	BObjectList<BMessenger *>		fTargets;
	// Use a one-dimensional array for the board to save some space.
	uint32 *						fBoard;
	uint32							fSizeX, fSizeY;
};

#endif // GAME_H
