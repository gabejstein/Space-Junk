#include <SDL.h>
#include "constants.h"

typedef struct GameObject GameObject;

typedef struct
{
	float x;
	float y;
}Vec2;

struct GameObject
{
	Vec2 position;
	Vec2 velocity;
	int w;
	int h;
	char* tag; //To identify the object type.
	float moveSpeed;
	SDL_Texture* texture;
	int isActive;
};

typedef struct
{
	void (*start)();
	void (*update)();
	void (*render)();
	void (*cleanup)();
}GameState;