/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <Looper.h>
#include <Messenger.h>

class BMessage;
class Game;

enum
{
	H2048_GAME_STARTED		= '48GS',
	H2048_GAME_ENDED		= '48GE',
	H2048_BOARD_CHANGED		= '48BC'
};

class GameBoard : public BLooper
{
public:
						GameBoard(Game *target);
	virtual				~GameBoard();
			void		MessageReceived(BMessage *message);

protected:
	// TODO: Pass the correct parameters and make a BMessage for them.
	virtual	void		gameStarted() = 0;
	virtual	void		gameEnded() = 0;
	virtual	void		boardChanged() = 0;

protected:
	Game *			fTarget;

private:
	BMessenger		fToSelf;
};

#endif // GAME_BOARD_H
