#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "game.h"

#define MAX_NAME_LENGTH 8
#define MAX_HIGH_SCORES 5

typedef struct
{
	char name[MAX_NAME_LENGTH];
	int score;
	int isRecent;
}Score;

void AddNewScore();
GameState* GetScoreBoardState(void);
void InitScoreBoard(void);
void SetLatestScore(int newScore);

#endif