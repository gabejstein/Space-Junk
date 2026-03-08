#include <SDL.h>
#include "constants.h"

typedef enum
{
	TAG_PLAYER,
	TAG_JUNK,
	TAG_BULLET,
	TAG_SMOKE
}OBJECT_TAG;

typedef struct
{
	float x;
	float y;
}Vec2;

typedef struct GameObject
{
	Vec2 position;
	Vec2 velocity;
	int w;
	int h;
	OBJECT_TAG tag; //To identify the object type.
	float moveSpeed;
	SDL_Texture* texture;
	int isActive;
}GameObject;

typedef struct
{
	void (*start)();
	void (*update)();
	void (*render)();
	void (*cleanup)();
}GameState;