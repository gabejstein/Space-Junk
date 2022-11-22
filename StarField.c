#include "Starfield.h"

Star* stars;

float starVelocity = 60.0;

void InitStarField(void)
{
	stars = (Star*)malloc(sizeof(Star) * MAX_STARS);

	if (stars == NULL) { printf("Could not allocate for stars.\n"); exit(1); }

	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].position.x =  (rand() % SCREEN_WIDTH);
		stars[i].position.y = (rand() % SCREEN_HEIGHT);
		stars[i].velocity.y = starVelocity * ((rand()%3)+1);
	}
}

void UpdateStarField(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
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
	int i;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (i = 0; i < MAX_STARS; i++)
	{

		SDL_RenderDrawLine(renderer, stars[i].position.x, stars[i].position.y, stars[i].position.x, stars[i].position.y-3);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void DestroyStarField(void)
{
	printf("Destroying Stars\n");
	free(stars);
}