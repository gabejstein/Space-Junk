#include "collision.h"

int CollidedRect(SDL_Rect A, SDL_Rect B)
{
	return ((A.x <= B.x + B.w) && (A.x+A.w >= B.x) && (A.y <= B.y + B.h) && (A.y+A.h >= B.y));
}