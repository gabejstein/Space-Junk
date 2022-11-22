#ifndef INPUT_H
#define INPUT_H

#include "game.h"

extern char textInputBuffer[MAX_TEXT_LINES];

void SetKeyboardPresses(SDL_Event event);

int IsKeyDown(SDL_Scancode key);
void ResetKey(int scanCode);
void SetTextInput(SDL_Event event);

#endif