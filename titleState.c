#include "titleState.h"

SDL_Texture* titleCard;

static void start()
{
	titleCard = IMG_LoadTexture(renderer, "Assets/Sprites/title.png");
	
}

static void update()
{
	if (IsKeyDown(SDL_SCANCODE_RETURN))
	{
		ChangeStates(GetPlayState());
	}
}

static void render()
{
	BlitTexture(titleCard, 0, 0);

	//Blink the press start text
	if ((SDL_GetTicks() / 200) % 2 == 0)
	{
		DrawTextToCenter("PRESS START");
	}
}

static void cleanup()
{
	SDL_DestroyTexture(titleCard);
}

GameState* GetTitleState(void)
{
	GameState* newState = (GameState*) malloc(sizeof(GameState));

	if (newState == NULL)
	{
		printf("Could not allocate memory for new state.\n");
		exit(1);
	}

	newState->start = start;
	newState->update = update;
	newState->render = render;
	newState->cleanup = cleanup;

	return newState;
}