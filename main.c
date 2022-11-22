#include <stdio.h>
#include "game.h"
#include <Windows.h>

int main(int argc, char* argv[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	InitGame();

	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	Cleanup();
	
	return 0;
}