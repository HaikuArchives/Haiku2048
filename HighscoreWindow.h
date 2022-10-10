/*
 * Copyright 2022, Harshit Sharma <harshits908@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef HIGHSCORE_WINDOW_H
#define HIGHSCORE_WINDOW_H


#include <Window.h>

class BStringView;
class BTextControl;

class HighscoreWindow : public BWindow{

public:
			HighscoreWindow(const char* oldHighscorer, const int32 oldHighscore, const int32 newHighScore);
			~HighscoreWindow();

			bool		QuitRequested();
			void		MessageReceived(BMessage *message);

private:

		BStringView*	fNewHighscoreText;
		BTextControl*	fInputBox;
};


#endif // _H
