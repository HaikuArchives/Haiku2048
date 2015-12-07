/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#include <Application.h>
#include "MainWindow.h"

class App : public BApplication
{
public:
			App();
};

App::App()
	:
	BApplication("application/x-vnd.Haiku-Haiku2048")
{
	(new MainWindow())->Show();
}

int main()
{
	App app;
	app.Run();
	return 0;
}
