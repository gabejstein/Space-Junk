#include "game.h"

SDL_Window* window;
SDL_Renderer* renderer;

int isRunning;

GameState* currentState;

float deltaTime = 0.0;
float lastFrameUpdate = 0.0;

void ChangeStates(GameState* newState)
{
	if (currentState->cleanup != NULL)
	{
		currentState->cleanup();
		free(currentState);
	}

	currentState = newState;

	if(currentState->start!=NULL)
		currentState->start();
}

int StartSDL()
{
	printf("Starting application.\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Something went wrong.\n");
		return 0;
	}

	window = SDL_CreateWindow("Space Junk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Could not create window.\n");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	IMG_Init(IMG_INIT_PNG);

	//Initialize mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Couldn't initialize SDL Mixer\n");
		return 0;
	}

	Mix_AllocateChannels(MAX_SND_CHANNELS);

	return 1;
}
void InitGame()
{
	isRunning = StartSDL();

	SDL_ShowCursor(0);

	LoadFontTexture("Assets/Sprites/font.png");

	InitScoreBoard();
	InitStarField();
	InitSound();

	PlayMusic(1);

	currentState = GetTitleState();
	currentState->start();
}
void ProcessInput()
{
	SDL_Event event;

	memset(textInputBuffer, '\0', MAX_TEXT_LINES);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = 0;
			break;
		case SDL_KEYDOWN:
			SetKeyboardPresses(event);
			break;
		case SDL_KEYUP:
			break;
		case SDL_TEXTINPUT:
			strncpy(textInputBuffer, event.text.text, MAX_TEXT_LINES);
			break;
		}
	}
}
void Render()
{
	SDL_RenderClear(renderer);

	
	currentState->render();

	SDL_RenderPresent(renderer);
}
void Update()
{
	int waitTime = MILLISECS_PER_FRAME - (SDL_GetTicks()-lastFrameUpdate);

	if (waitTime > 0)
		SDL_Delay(waitTime);

	deltaTime = (SDL_GetTicks() - lastFrameUpdate) / 1000;

	lastFrameUpdate = SDL_GetTicks();

	currentState->update();
}

void Cleanup()
{
	printf("Quitting application and cleaning up.\n");

	free(currentState);
	DestroyStarField();

	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}