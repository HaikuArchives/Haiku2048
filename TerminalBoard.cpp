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

TerminalBoard::TerminalBoard(Game *target)
	:
	GameBoard(target)
{
}

TerminalBoard::~TerminalBoard()
{
}

void
TerminalBoard::gameStarted()
{
	showBoard();
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

void
TerminalBoard::showBoard()
{
	uint32 width = 1;

	for (uint32 x = 0; x < fTarget->SizeX(); x++)
	{
		for (uint32 y = 0; y < fTarget->SizeY(); y++)
		{
			uint32 digl = digits(fTarget->BoardAt(x, y));
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

			std::cout
				<< std::setw(width)
				<< (tileValue != 0 ? (1 << tileValue) : 0)
				<< "|";
		}
		std::cout << '\n';
	}
	std::cout << dashes << std::endl;
}
