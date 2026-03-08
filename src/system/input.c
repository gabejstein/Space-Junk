#include "input.h"

Uint8* KeyboardPresses;

char textInputBuffer[MAX_TEXT_LINES];

void SetKeyboardPresses(SDL_Event event)
{
	//TODO: Set value in array based on appropriate scancode.
	KeyboardPresses = SDL_GetKeyboardState(0);
}

void SetKeyboardUp(SDL_Event event)
{

}

void ResetKey(int scanCode)
{
	KeyboardPresses[scanCode] = 0;
}

int IsKeyDown(SDL_Scancode key)
{
	if(KeyboardPresses!=0)
		return KeyboardPresses[key];

	return 0;
}

void SetTextInput(SDL_Event* event)
{
	//strncpy(textInputBuffer, event.text.text,MAX_TEXT_LINES);
}