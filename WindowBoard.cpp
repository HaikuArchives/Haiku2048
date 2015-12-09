/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "WindowBoard.h"
#include "Game.h"
#include "NumberView.cpp"

#include <Box.h>
#include <StringView.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Alert.h>
#include <Rect.h>

GameWindow::GameWindow(WindowBoard *master)
	:
	BWindow(BRect(100, 100, 500, 400), "Haiku2048", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE),
	fMaster(master)
{
	BButton *newGameButton = new BButton("newgame", "New Game",
		new BMessage(H2048_NEW_GAME));

	fScore = new BStringView("score", "Score: 0");

	fBoard = new BGridLayout(5.0, 5.0);

	NumberView *num = new NumberView(1024, 100, 100);
	NumberView *num2 = new NumberView(1024, 100, 100);

//	num->ResizeTo(10, 10);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL, 50.0)
			.Add(newGameButton)
			.Add(fScore)
			.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fBoard)
		.AddGlue();


	fBoard->AddView(num, 0, 0);
	fBoard->AddView(num2, 1, 1);

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
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

void
GameWindow::showBoard()
{
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

	(new BAlert("Title", "Game Started", "OK"))->Go();
}

void
WindowBoard::gameEnded()
{
	(new BAlert("Title", "Game Ended", "OK"))->Go();
}

void
WindowBoard::moveMade()
{
	(new BAlert("Title", "Move Made", "OK"))->Go();
}
