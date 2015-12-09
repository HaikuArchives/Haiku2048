/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include <String.h>
#include "Game.h"
#include "TerminalBoard.h"

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

int32 control(void *data);

TerminalBoard::TerminalBoard(Game *target)
	:
	GameBoard(target)
{
	fControlID = spawn_thread(&control, "TerminalBoard control keeper",
		B_NORMAL_PRIORITY, (void *)target);
}

TerminalBoard::~TerminalBoard()
{
}

void
TerminalBoard::gameStarted()
{
	showBoard();
	resume_thread(fControlID);
}

void
TerminalBoard::gameEnded()
{
	std::cout << "Game has ended." << std::endl;
}

void
TerminalBoard::moveMade()
{
	showBoard();
}

uint32
digits(uint32 num)
{
	uint32 result = 1;
	while (num > 10)
	{
		result++;
		num /= 10;
	}
	return result;
}

int32
control(void *data)
{
	Game *target = (Game *)data;
	BMessenger messenger(NULL, target);
	while (true)
	{
		int32 c = std::cin.get();
		if (c != 'w' && c != 'a' && c != 's' && c != 'd')
			continue;
		std::cout << std::endl;
		BMessage move(H2048_MAKE_MOVE);
		move.AddInt32("direction", c);
		messenger.SendMessage(&move);
	}
}

void
TerminalBoard::showBoard()
{
	uint32 width = 1;

	for (uint32 x = 0; x < fTarget->SizeX(); x++)
	{
		for (uint32 y = 0; y < fTarget->SizeY(); y++)
		{
			uint32 digl = digits(1 << fTarget->BoardAt(x, y));
			width = width > digl ? width : digl;
		}
	}

	BString dashes;
	uint32 numDashes = fTarget->SizeX() * (width + 1) + 1;
	for (uint32 i = 0; i < numDashes; i++)
	{
		dashes << "-";
	}

	for (uint32 y = 0; y < fTarget->SizeY(); y++)
	{
		// Header
		std::cout << dashes << "\n|";
		for (uint32 x = 0; x < fTarget->SizeX(); x++)
		{
			uint32 tileValue = fTarget->BoardAt(x, y);
			std::cout << setw(width);
			if (tileValue != 0)
				std::cout << (1 << tileValue);
			else
				std::cout << " ";
			std::cout << "|";
		}
		std::cout << '\n';
	}
	std::cout << dashes << "\nScore: " << fTarget->Score() << std::endl;
}
