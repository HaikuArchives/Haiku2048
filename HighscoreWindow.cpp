/*
 * Copyright 2022, Harshit Sharma <harshits908@gmail.com>
 * Copyright 2023, Johan Wagenheim
 * This file is distributed under the terms of the MIT license
 */

#include "HighscoreWindow.h"
#include "WindowBoard.h"
#include "Game.h"

#include <Catalog.h>
#include <string>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Messenger.h>
#include <Button.h>
#include <String.h>
#include <StringView.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "HighscoreWindow"

HighscoreWindow::HighscoreWindow(const char* oldHighscorer, const int32 oldHighscore,
	const int32 newHighscore, const BMessenger &messenger)
	:
	BWindow(BRect(200, 200, 1,1), 0, B_MODAL_WINDOW, B_NOT_CLOSABLE | B_NOT_RESIZABLE),
				fMessenger(messenger)
{
	BStringView* congratulations = new BStringView("congratulations",
											B_TRANSLATE("Congratulations!"));
	congratulations->SetFont(be_bold_font);
	congratulations->SetFontSize(25);
	congratulations->SetAlignment(B_ALIGN_HORIZONTAL_CENTER);

	BString newHighscoreText = B_TRANSLATE("New high-score: %newhighscore%");
	newHighscoreText.ReplaceFirst("%newhighscore%", std::to_string(newHighscore).c_str());
	BStringView* newHighScore = new BStringView("newHighScore", newHighscoreText.String());
	newHighScore->SetFont(be_bold_font);
	newHighScore->SetFontSize(20);
	newHighScore->SetAlignment(B_ALIGN_HORIZONTAL_CENTER);

	BStringView* inputlabel = new BStringView("inputlabel",
										B_TRANSLATE("Please enter your name:"));
	inputlabel->SetAlignment(B_ALIGN_HORIZONTAL_CENTER);
	inputlabel->SetFont(be_bold_font);

	fInputBox = new
		BTextControl("NameInput", NULL, NULL, new BMessage(H2048_NAME_ENTERED));
	fInputBox->SetText(B_TRANSLATE("Anonymous"));
	fInputBox->SetAlignment(B_ALIGN_HORIZONTAL_CENTER,B_ALIGN_HORIZONTAL_CENTER);
	fInputBox->SetModificationMessage(new BMessage(H2048_CHANGE_NAME));

	BString previousWinner;
	previousWinner << B_TRANSLATE("This is the first record!");
	if (oldHighscorer[0]) {
		// Replace with previous winner and score difference
		previousWinner = B_TRANSLATE("You've beaten the previous record holder,\n"
										"%oldHighscorer%, by %deltaScore%.");
		previousWinner.ReplaceFirst("%oldHighscorer%", oldHighscorer);
		previousWinner.ReplaceFirst("%deltaScore%",
					std::to_string(newHighscore - oldHighscore).c_str());
	}

	BStringView* previousWinnerText = new BStringView("previousWinner", previousWinner.String());
	previousWinnerText->SetAlignment(B_ALIGN_HORIZONTAL_CENTER);

	BButton *saveName = new BButton("savename", B_TRANSLATE("Hooray!"),
		new BMessage(H2048_NAME_ENTERED));
	saveName->SetFontSize(15);
	saveName->SetFont(be_bold_font);
	saveName->ResizeToPreferred();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL)
			.Add(congratulations)
			.Add(newHighScore)
			.Add(previousWinnerText)
			.AddStrut(2)
			.Add(inputlabel)
			.Add(fInputBox)
			.Add(saveName)
			.AddGlue()
			.End();

	this->ResizeToPreferred();
	fInputBox->MakeFocus();
}


bool
HighscoreWindow::QuitRequested()
{
	return true;
}


HighscoreWindow::~HighscoreWindow()
{
}


void
HighscoreWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case H2048_NAME_ENTERED:
		{
			// Set a default value if empty
			if (!fInputBox->Text()[0])
				fInputBox->SetText(B_TRANSLATE("Anonymous"));

			BMessage req(H2048_NAME_REQUESTED);
			req.AddString("playername", fInputBox->Text());
			fMessenger.SendMessage(&req);
			Quit();
			break;
		}
		case H2048_CHANGE_NAME:
		{
			BString name = fInputBox->Text();
			int32 count = name.CountChars();
			if (count > 25)
				fInputBox->TextView()->Delete(count - 1, count);
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}
