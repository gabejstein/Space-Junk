#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "game.h"

void AddNewScore();
GameState* GetScoreBoardState(void);
void InitScoreBoard(void);
void SetLatestScore(int newScore);

#endif