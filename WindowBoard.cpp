/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "WindowBoard.h"

#include "Game.h"
#include "NumberView.cpp"

#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <Rect.h>
#include <String.h>
#include <StringView.h>

GameWindow::GameWindow(WindowBoard *master)
	:
	BWindow(BRect(100, 100, 500, 400), "Haiku2048", B_TITLED_WINDOW, 0),
	fMaster(master)
{
	BButton *newGameButton = new BButton("newgame", "New Game",
		new BMessage(H2048_NEW_GAME));

	undoButton = new BButton("undomove", "Undo last move",
		new BMessage(H2048_UNDO_MOVE));

	fScore = new BStringView("score", "Score: 0");

	fBoard = new BGridLayout();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(newGameButton)
			.Add(undoButton)
			.Add(fScore)
			.End()
		.Add(fBoard);

	uint32 sizeX = fMaster->fTarget->SizeX();
	uint32 sizeY = fMaster->fTarget->SizeY();

	fViews = new NumberView *[sizeX * sizeY];

	for (uint32 x = 0; x < sizeX; x++)
	{
		for (uint32 y = 0; y < sizeY; y++)
		{
			NumberView *num = new NumberView(0);
			fViews[x * sizeY + y] = num;
			fBoard->AddView(num, x, y);
		}
	}
	ResizeToPreferred();
	BRect rect = Bounds();
	prevWidth = rect.Width();
	prevHeight = rect.Height();
	defaultWidth = rect.Width();
	defaultHeight = rect.Height();
}

GameWindow::~GameWindow()
{
	fMaster->fWindow = NULL;
	delete [] fViews;
}

bool
GameWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return true;
}

void
GameWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case H2048_NEW_GAME:
		{
			BMessenger game(NULL, fMaster->fTarget);
			game.SendMessage(message);
			break;
		}
		case H2048_WINDOW_SHOW:
		{
			showBoard();
			break;
		}
		case H2048_UNDO_MOVE:
		{
			BMessenger game(NULL, fMaster->fTarget);
			game.SendMessage(message);
			break;
		}
		case B_KEY_DOWN:
		{
			const char *data;
			ssize_t length;
			if (fMaster->fSending && message->FindData("bytes", B_STRING_TYPE,
				(const void **)&data, &length) == B_OK && data[0] >= 28
				&& data[0] <= 31)
			{
				GameMove m;

				switch (data[0])
				{
					case 30: //Up
						m = Up;
						break;
					case 28: // Left
						m = Left;
						break;
					case 31: // Down
						m = Down;
						break;
					case 29: // Right
						m = Right;
						break;
				}
				BMessage move(H2048_MAKE_MOVE);
				move.AddInt32("direction", m);
				BMessenger messenger(NULL, fMaster->fTarget);
				messenger.SendMessage(&move);
			}
			BWindow::MessageReceived(message);
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

void
GameWindow::showBoard()
{
	Game *target = fMaster->fTarget;
	uint32 sizeX = target->SizeX();
	uint32 sizeY = target->SizeY();

	for (uint32 x = 0; x < sizeX; x++)
	{
		for (uint32 y = 0; y < sizeY; y++)
		{
			fViews[x * sizeY + y]->SetNumber(1 << target->BoardAt(x, y));
			fViews[x * sizeY + y]->Invalidate();
		}
	}

	BString score;
	score << "Score: " << fMaster->fTarget->Score();
	fScore->SetText(score.String());
}

void
GameWindow::FrameResized(float width, 
			 float height)
{
	// We don't want the user to scale the window so small that
	// there's no space for the buttons.
	if (width < defaultWidth) {
		ResizeTo(defaultWidth, defaultHeight);
		SetScale(1);
	}
	// Maintain the same width:height ratio
	else {
		float ratio = width/prevWidth;
		SetScale(ratio);
		prevWidth = width;
		prevHeight = height;
		ResizeTo(width, height*ratio);
	}
}

WindowBoard::WindowBoard(Game *target)
	:
	GameBoard(target),
	fSending(false)
{
	fWindow = new GameWindow(this);
	fWindow->Show();
}

WindowBoard::~WindowBoard()
{
	delete fWindow;
}

void
WindowBoard::gameStarted()
{
	BMessenger messenger(NULL, fWindow);
	messenger.SendMessage(H2048_WINDOW_SHOW);
	fSending = true;
}

void
WindowBoard::gameEnded()
{
	fSending = false;
	(new BAlert("Title", "Game Ended", "OK"))->Go();
}

void
WindowBoard::boardChanged()
{
	BMessenger messenger(NULL, fWindow);
	messenger.SendMessage(H2048_WINDOW_SHOW);
}
