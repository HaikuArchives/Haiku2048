/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "WindowBoard.h"
#include "Game.h"
#include "NumberView.cpp"

#include <Messenger.h>
#include <Box.h>
#include <StringView.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Alert.h>
#include <Rect.h>

GameWindow::GameWindow(WindowBoard *master)
	:
	BWindow(BRect(100, 100, 500, 400), "Haiku2048", B_TITLED_WINDOW, 0),
	fMaster(master)
{
	BButton *newGameButton = new BButton("newgame", "New Game",
		new BMessage(H2048_NEW_GAME));

	fScore = new BStringView("score", "Score: 0");

	fBoard = new BGridLayout(5.0, 5.0);

	NumberView *num = new NumberView(1024, 0, 0);
	NumberView *num2 = new NumberView(1024, 0, 0);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(newGameButton)
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
			NumberView *num = new NumberView(0, 0, 0);
			fViews[x * sizeY + y] = num;
			fBoard->AddView(num, x, y);
		}
	}

	ResizeToPreferred();
}

GameWindow::~GameWindow()
{
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
			showBoard();
			break;
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
}

WindowBoard::WindowBoard(Game *target)
	:
	GameBoard(target),
	fWindow(this)
{
	fWindow.Show();
}

WindowBoard::~WindowBoard()
{
}

void
WindowBoard::gameStarted()
{
	BMessenger messenger(NULL, &fWindow);
	messenger.SendMessage(H2048_WINDOW_SHOW);
}

void
WindowBoard::gameEnded()
{
	(new BAlert("Title", "Game Ended", "OK"))->Go();
}

void
WindowBoard::moveMade()
{
	BMessenger messenger(NULL, &fWindow);
	messenger.SendMessage(H2048_WINDOW_SHOW);
}
