/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#ifndef TERMINAL_BOARD_H
#define TERMINAL_BOARD_H

#include "GameBoard.h"

class TerminalBoard : public GameBoard
{
public:
						TerminalBoard(Game *target);
						~TerminalBoard();

protected:
			void		gameStarted();
			void		gameEnded();
			void		nameRequest();
			void		boardChanged(bool canUndo);

private:
			void		showBoard();

private:
	thread_id	fControlID;
};

#endif // TERMINAL_BOARD_H
