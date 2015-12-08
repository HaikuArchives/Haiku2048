/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <Looper.h>

class BMessage;
class Game;

class GameBoard : public BLooper
{
public:
						GameBoard(Game *target);
	virtual				~GameBoard();
			void		MessageReceived(BMessage *message);

protected:
	// TODO: Pass the correct parameters and make a BMessage for them.
	virtual	void		GameStarted() = 0;
	virtual	void		GameEnded() = 0;
	virtual	void		MoveMade() = 0;

private:
	Game *		fTarget;

};

#endif GAME_BOARD_H
