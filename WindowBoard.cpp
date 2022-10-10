/*
 * Copyright (c) 2015 Markus Himmel
 * Copyright 2022, Harshit Sharma <harshits908@gmail.com>
 * This file is distributed under the terms of the MIT license
 */

#include "WindowBoard.h"

#include "Game.h"
#include "NumberView.cpp"

#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <Rect.h>
#include <String.h>
#include <StringView.h>
#include <IconUtils.h>
#include <Resources.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "WindowBoard"

GameWindow::GameWindow(WindowBoard *master)
	:
	BWindow(BRect(100, 100, 500, 400), B_TRANSLATE_SYSTEM_NAME("Haiku2048"), B_TITLED_WINDOW, 0),
	fMaster(master)
{	
	fIconUndo = initIcon("icon_undo.hvif");
	fIconNew = initIcon("icon_new.hvif");
	
	BButton *newGameButton = new BButton("newgame", "",
		new BMessage(H2048_NEW_GAME));
	newGameButton->SetIcon(fIconNew);
	undoButton = new BButton("undomove", "",
		new BMessage(H2048_UNDO_MOVE));
	undoButton->SetIcon(fIconUndo);
	undoButton->SetEnabled(false);

	fScore_Highest = new BStringView("score_highest", B_TRANSLATE("High-score: 0"));
	fScore = new BStringView("score", B_TRANSLATE("Score: 0"));
	fHighscoreName = new BStringView("highscore_name","");

	fBoard = new BGridLayout();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(newGameButton)
			.Add(undoButton)
			.AddStrut(5)
			.AddGroup(B_VERTICAL, -15)
				.Add(fHighscoreName)
				.AddGroup(B_HORIZONTAL)
					.AddGlue()
					.Add(fScore_Highest)
					.End()
				.End()
			.AddGlue()
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
	prevWidth = rect.Width() - 20; 	// due to changes in font size, some of the boxes at 
									// the bottom would be concealed. This is to expand window size.
	prevHeight = rect.Height();
	defaultWidth = rect.Width();
	defaultHeight = rect.Height();
}

BBitmap*
GameWindow::initIcon(const char* iconName) {
	BBitmap* icon = NULL;
	BResources* resources = BApplication::AppResources();
	size_t size;
	const void* rawIcon = resources->LoadResource('VICN', iconName, &size);
	icon = new BBitmap(BRect(0, 0, 31, 31), B_RGBA32);
	BIconUtils::GetVectorIcon((const uint8*)rawIcon, size, icon);
	return icon;
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
			bool canUndo = false;
			message->FindBool("canUndo", &canUndo);
			showBoard(canUndo);
			break;
		}
		case H2048_UNDO_MOVE:
		{
			if (!fMaster->fSending) {
				// fMaster->fSending is false when:
				// * The game hasn't started (unlikely, and the button is disabled anyway)
				// * Game over
				// In case of game over, we have to make it true, so that
				// subsequent keypressed are acknowledged
				fMaster->fSending = true;
			}
			
			BMessenger game(NULL, fMaster->fTarget);
			game.SendMessage(message);
			break;
		}
		case B_KEY_DOWN:
		{
			const char *data;
			ssize_t length;
			if (fMaster->fSending
				&& message->FindData("bytes", B_STRING_TYPE, (const void **)&data, &length) == B_OK
				&& (data[0] == 'u' || (data[0] >= 28 && data[0] <= 31)))
			{
				if (data[0] == 'u') {
					PostMessage(H2048_UNDO_MOVE);
					break;
				}

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
		case H2048_REQUEST_NAME:
		{
			HighscoreWindow highscoreWindow(fMaster->fTarget->Username(),
				fMaster->fTarget->Score_Highest(), fMaster->fTarget->Score());
			highscoreWindow.Show();
			highscoreWindow.Activate();
			
			break;
		}
		case H2048_SET_NAME:
		{
			BMessage req(H2048_NAME_REQUESTED);
			req.AddString("playername", fInputBox->Text());
			BMessenger messenger(NULL, fMaster->fTarget);
			messenger.SendMessage(&req);
			ResizeBy(0.0, -35.0);
			RemoveChild(FindView("reqbox"));
			delete fInputBox;
			fInputBox = NULL;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

void
GameWindow::showBoard(bool canUndo)
{
	undoButton->SetEnabled(canUndo);

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

	BString highscore_name;
	highscore_name << B_TRANSLATE("High-score:");
	if (fMaster->fTarget->Username()[0]) {
		highscore_name = B_TRANSLATE("High-score by %username%:");
		highscore_name.ReplaceFirst("%username%", fMaster->fTarget->Username());
	}
	fHighscoreName->SetText(highscore_name.String());

	BString score_highest;
	score_highest << fMaster->fTarget->Score_Highest();
	fScore_Highest->SetText(score_highest.String());
	fScore_Highest->SetFont(be_bold_font);
	fScore_Highest->SetFontSize(15);

	BString score;
	score << fMaster->fTarget->Score();
	fScore->SetText(score.String());
	fScore->SetFont(be_bold_font);
	fScore->SetFontSize(35);
}

void
GameWindow::FrameResized(float width, 
			 float height)
{
	// We don't want the user to scale the window so small that
	// there's no space for the buttons.
	if (width < defaultWidth) {
		ResizeTo(defaultWidth, defaultHeight);
		width=defaultWidth;
	}
	// Maintain the same width:height ratio
	else {
		float ratio = width/prevWidth;

		ResizeTo(width, height*ratio);

	}

	//Don't scale on first resize
	if(prevWidth >= defaultWidth){
		ScaleBy(width/prevWidth);
	}

	prevWidth = width;
	prevHeight = height;
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
	BMessage redraw(H2048_WINDOW_SHOW);
	redraw.AddBool("canUndo", false);

	BMessenger messenger(NULL, fWindow);
	messenger.SendMessage(&redraw);

	fSending = true;
}

void
WindowBoard::gameEnded()
{
	fSending = false;
	(new BAlert(B_TRANSLATE("End of game"), B_TRANSLATE(
		"All tiles locked in place\n"
		"Like leaves on a frozen lake\n"
		"The game has ended"), B_TRANSLATE("OK")))->Go();
}

void
WindowBoard::boardChanged(bool canUndo)
{
	BMessage redraw(H2048_WINDOW_SHOW);
	redraw.AddBool("canUndo", canUndo);

	BMessenger messenger(NULL, fWindow);
	messenger.SendMessage(&redraw);
}

void
WindowBoard::nameRequest()
{
	BMessenger messenger(NULL, fWindow);
	messenger.SendMessage(H2048_REQUEST_NAME);
}
