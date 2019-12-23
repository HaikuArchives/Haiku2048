/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license.
 */

#ifndef GAME_H
#define GAME_H

#include <vector>

#include <Looper.h>
#include <SupportDefs.h>

#define HIGHSCORE_FILENAME "Highscore"
#define HAIKU2048_DIRECTORY "Haiku2048"
#define SAVEFILE_FILENAME "Save"

class BMessage;
class BMessenger;

enum GameMove
{
	Left = 'a',
	Right = 'd',
	Up = 'w',
	Down = 's'
};

enum
{
	H2048_NEW_GAME		= '48NG',
	H2048_NAME_REQUESTED= '48NR',
	H2048_UNDO_MOVE		= '48UM',
	H2048_MAKE_MOVE		= '48MM',
	H2048_LOAD_GAME		= '48LG',
	H2048_SAVE_GAME		= '48SG'
};

class Game : public BLooper
{
public:
						Game(uint32 sizeX, uint32 sizeY);
						~Game();
			void		MessageReceived(BMessage *message);

			void		StartWatching(BMessenger *target);
			void		StopWatching(BMessenger *target);

			uint32		BoardAt(uint32 x, uint32 y) const;

			uint32		Score() const;
			uint32		Score_Highest() const;
			uint32		SizeX() const;
			uint32		SizeY() const;
			const char *Username() const;

private:
			void		newGame();
			void		makeMove(GameMove direction);
			void		undoMove();
			void		copyBoard(uint32 *from, uint32 *to);
			void		broadcastMessage(BMessage &msg);
			void		save();
			bool		load();
			uint32 *	boardAt(uint32 x, uint32 y);
			uint32		newTile();
			bool		gameOver();
			
			void		writeHighscore();

private:
	std::vector<BMessenger *>	fTargets;
	// Use a one-dimensional array for the board to save some space.
	uint32 *					fBoard;

	// Saves board state and score at previous move
	uint32 *					fPreviousBoard;
	uint32						fPreviousScore;
	// fCanUndo is false when:
	// * There is no previous state to undo (right after new game)
	// * User has already undone (can only undo once)
	bool						fCanUndo;

	uint32						fSizeX, fSizeY;
	bool						fInGame;
	uint32						fScore;
	uint32						fScore_Highest;
	char						fUsername[32];
	char						fPlayername[32];
	char						fHighscore_path[128];
	char						fSaveFile_path[128];
};

#endif // GAME_H
