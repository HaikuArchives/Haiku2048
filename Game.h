/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#ifndef GAME_H
#define GAME_H

#include <vector>

#include <Looper.h>
#include <SupportDefs.h>

class BMessage;
class BMessenger;

enum GameMove
{
	Left = 'a',
	Right = 'd',
	Up = 'w',
	Down = 's'
};

enum
{
	H2048_NEW_GAME		= '48NG',
	H2048_MAKE_MOVE		= '48MM'
};

class Game : public BLooper
{
public:
						Game(uint32 sizeX, uint32 sizeY);
						~Game();
			void		MessageReceived(BMessage *message);

			void		StartWatching(BMessenger *target);
			void		StopWatching(BMessenger *target);

			uint32		BoardAt(uint32 x, uint32 y) const;

			uint32		Score() const;
			uint32		SizeX() const;
			uint32		SizeY() const;

private:
			void		newGame();
			void		makeMove(GameMove direction);
			void		broadcastMessage(BMessage &msg);
			uint32 *	boardAt(uint32 x, uint32 y);
			uint32		newTile();
			bool		gameOver();

private:
	std::vector<BMessenger *>		fTargets;
	// Use a one-dimensional array for the board to save some space.
	uint32 *						fBoard;
	uint32							fSizeX, fSizeY;
	bool							fInGame;
	uint32							fScore;
};

#endif // GAME_H
