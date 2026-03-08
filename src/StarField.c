#include "Starfield.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STARS 500
#define STAR_VELOCITY 60.0f

typedef struct Star
{
	Vec2 position;
	Vec2 velocity;
}Star;

static Star stars[MAX_STARS];

void InitStarField(void)
{
	for (int i = 0; i < MAX_STARS; i++)
	{
		stars[i].position.x =  (rand() % SCREEN_WIDTH);
		stars[i].position.y = (rand() % SCREEN_HEIGHT);
		stars[i].velocity.y = STAR_VELOCITY * ((rand()%3)+1);
	}
}

void UpdateStarField(void)
{

	for (int i = 0; i < MAX_STARS; i++)
	{
		stars[i].position.y += stars[i].velocity.y * deltaTime;

		if (stars[i].position.y > SCREEN_HEIGHT)
		{
			stars[i].position.x = (rand() % SCREEN_WIDTH);
			stars[i].position.y = -5.0;
		}
	}

}
void RenderStarField(void)
{

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < MAX_STARS; i++)
	{

		SDL_RenderDrawLine(renderer, stars[i].position.x, stars[i].position.y, stars[i].position.x, stars[i].position.y-3);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}