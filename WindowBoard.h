/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#ifndef WINDOW_BOARD_H
#define WINDOW_BOARD_H

#include "GameBoard.h"

#include <Window.h>
#include <Bitmap.h>
class Game;
class BButton;
class NumberView;
class WindowBoard;
class BStringView;
class BGridLayout;
class BTextControl;

enum
{
	H2048_WINDOW_SHOW = '48WS',
	H2048_SET_NAME    = '48SN'
};

/*
 * This is split up into two classes in order to prevent multiple inheritance
 * and more specifically diamond inheritance on BLooper
 */
class GameWindow : public BWindow
{
public:
						GameWindow(WindowBoard *master);
						~GameWindow();
						
			float 		prevWidth;
			float 		prevHeight;
			float 		defaultWidth;
			float 		defaultHeight;
			
			bool		QuitRequested();
			void		FrameResized(float width, float height);
			void		MessageReceived(BMessage *message);
private:
			void		showBoard(bool canUndo);
			BBitmap*	initIcon(const char* iconName);

private:
	NumberView **		fViews;
	WindowBoard *		fMaster;
	BStringView *		fScore;
	BStringView *		fScore_Highest;
	BStringView *		fHighscoreName;
	BTextControl*       fInputBox;
	BGridLayout *		fBoard;
	BBitmap *			fIconNew;
	BBitmap * 			fIconUndo;

	// we have to control the state of it
	// (enabled / disabled)
	BButton * 			undoButton;
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
			void		nameRequest();
			void		boardChanged(bool canUndo);

private:
	bool				fSending;
	GameWindow *		fWindow;
};

#endif // WINDOW_BOARD_H
