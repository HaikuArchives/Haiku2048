/*
 * Copyright 2022, Harshit Sharma <harshits908@gmail.com>
 * This file is distributed under the terms of the MIT license
 */

#include "HighscoreWindow.h"

#include "Game.h"

#include <Catalog.h>
#include <string>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <StringView.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "HighscoreWindow"

static const uint32 kNameEntered = 'NmEn';

HighscoreWindow::HighscoreWindow(const char* oldHighscorer, const int32 oldHighscore, const int32 newHighscore)
	:
	BWindow(BRect(200, 200, 600, 450), B_TRANSLATE("Enter you High Score"), B_TITLED_WINDOW, 0)
{
	BStringView* congratulations = new BStringView("congratulations", B_TRANSLATE("Congratulations!"));
	congratulations->SetFont(be_bold_font);
	congratulations->SetFontSize(25);

	fInputBox = new
		BTextControl("NameInput", B_TRANSLATE("Please enter your name:"), NULL,
		new BMessage(kNameEntered));
	fInputBox->MakeFocus();

	BStringView* line1 = new BStringView("line1", B_TRANSLATE("You achieved a new highscore, beating the one"));
	BString line2Text = B_TRANSLATE("by %oldHighscorer% by %deltaScore%.");
	BString line3Text = B_TRANSLATE("Your new highscore is %newhighscore%");
	
	line2Text.ReplaceFirst("%oldHighscorer%", oldHighscorer);
	line2Text.ReplaceFirst("%deltaScore%", std::to_string(newHighscore - oldHighscore).c_str());
	line3Text.ReplaceFirst("%newhighscore%", std::to_string(newHighscore).c_str());
	BStringView* line2 = new BStringView("line2", line2Text.String());
	BStringView* line3 = new BStringView("line3", line3Text.String());

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL)
			.Add(congratulations)
			.AddStrut(5)
			.Add(line1)
			.Add(line2)
			.Add(line3)
			.AddStrut(5)
			.Add(fInputBox)
			.AddGlue()
			.End();
}

bool
HighscoreWindow::QuitRequested()
{
	return true;
}

HighscoreWindow::~HighscoreWindow()
{
	delete(fInputBox);
}

void
HighscoreWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case kNameEntered:
		{
			BMessage *req = new BMessage(H2048_NAME_REQUESTED);
			req->AddString("playername", fInputBox->Text());
			BMessenger messenger(this);
			messenger.SendMessage(req);
			QuitRequested();
			break;
		}
		default:
			break;
	}
}
