#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "structs.h"

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