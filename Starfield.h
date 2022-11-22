#ifndef STARFIELD_H
#define STARFIELD_H

#include "game.h"

#define MAX_STARS 500

typedef struct Star
{
	Vec2 position;
	Vec2 velocity;
}Star;

void InitStarField(void);
void UpdateStarField(void);
void RenderStarField(void);
void DestroyStarField(void);

#endif