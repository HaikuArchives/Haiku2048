/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#ifndef WINDOW_BOARD_H
#define WINDOW_BOARD_H

#include "GameBoard.h"
#include <Window.h>

class Game;
class WindowBoard;
class BStringView;
class BBox;

/*
 * This is split up into two classes in order to prevent multiple inheritance
 * and more specifically diamond inheritance on BLooper
 */
class GameWindow : public BWindow
{
public:
						GameWindow(WindowBoard *master);
						~GameWindow();

			void		MessageReceived(BMessage *message);

private:
			void		showBoard();

private:
	WindowBoard *		fMaster;
	BStringView *		fScore;
	BBox *				fBoard;
};


class WindowBoard : public GameBoard
{
friend class GameWindow;

public:
						WindowBoard(Game *target);
						~WindowBoard();

protected:
			void		gameStarted();
			void		gameEnded();
			void		moveMade();

private:
	GameWindow		fWindow;
};

#endif // WINDOW_BOARD_H
