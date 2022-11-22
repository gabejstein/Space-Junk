#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_Mixer.h"
#include "constants.h"
#include "graphics.h"
#include "structs.h"
#include "playState.h"
#include "titleState.h"
#include "input.h"
#include "scoreBoard.h"
#include "Starfield.h"
#include "sound.h"


extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int isRunning;
extern float deltaTime;

void ChangeStates(GameState* newState);

int StartSDL();
void InitGame();
void ProcessInput();
void Render();
void Update();
void Cleanup();

#endif