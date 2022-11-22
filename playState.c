#include "playState.h"
#include <time.h>
#include <stdlib.h>
#include "collision.h"

int playerScore;
int playerLives;

SDL_Texture* spaceShipTexture;
SDL_Texture* junkTexture[6];
SDL_Texture* projectileTexture;
SDL_Texture* smokeTexture;

GameObject ship;

GameObject objects[MAX_POOL_OBJECTS];
int oIndex = 0;

void UpdatePlayer();
void UpdateObjects();
void CheckCollisions();
void SpawnRandomObject();
void FireProjectile();
void RenderHUD();
void DeactiveAllObjects();
void ResetState();
void CreateExplosion();

float spawnInterval = 1;
float lastSpawnTime = 0.0;
float lastFiredTime = 0.0;

float fireRate = 0.3;
float junkSpeed = 600.0;
float projectileSpeed = 500.0;

int paused = 0;
int resetTimer;
int gameOver;

static void start()
{
	printf("Starting play state.\n");

	time_t t;
	srand((unsigned)time(&t));

	spaceShipTexture = IMG_LoadTexture(renderer, "Assets/Sprites/spiked_ship.png");

	junkTexture[0] = IMG_LoadTexture(renderer, "Assets/Sprites/rock.png");
	junkTexture[1] = IMG_LoadTexture(renderer, "Assets/Sprites/boot.png");
	junkTexture[2] = IMG_LoadTexture(renderer, "Assets/Sprites/crate.png");
	junkTexture[3] = IMG_LoadTexture(renderer, "Assets/Sprites/mailbox.png");
	junkTexture[4] = IMG_LoadTexture(renderer, "Assets/Sprites/can.png");
	junkTexture[5] = IMG_LoadTexture(renderer, "Assets/Sprites/drum.png");

	projectileTexture = IMG_LoadTexture(renderer, "Assets/Sprites/fire04.png");
	smokeTexture = IMG_LoadTexture(renderer, "Assets/Sprites/smoke_01.png");

	SDL_QueryTexture(spaceShipTexture, NULL, NULL, &ship.w, &ship.h);

	playerScore = 0;
	playerLives = 3;

	//To prevent going into pause right away
	ResetKey(SDL_SCANCODE_RETURN);

	ResetState();

}

void ResetState()
{
	ship.position.x = SCREEN_WIDTH / 2;
	ship.position.y = SCREEN_HEIGHT - 100;
	ship.moveSpeed = 250.0;
	ship.isActive = 1;

	resetTimer = FPS * 4;
	gameOver = 0;
	DeactiveAllObjects();
}

void DeactiveAllObjects()
{
	int i;
	for (i = 0; i < MAX_POOL_OBJECTS; i++)
		objects[i].isActive = 0;

	oIndex = 0;
}

static void update()
{
	if (IsKeyDown(SDL_SCANCODE_RETURN) && !gameOver)
	{
		paused = paused == 0 ? 1 : 0;
		ResetKey(SDL_SCANCODE_RETURN);
	}
		

	if (paused)
		return;

	if (ship.isActive == 0)
	{
		if (playerLives <= 0)
			gameOver = 1;

		if (--resetTimer <= 0)
		{
			//TODO: Reset the stage.
			if (!gameOver)
				ResetState();
			else
			{
				SetLatestScore(playerScore);
				ChangeStates(GetScoreBoardState());
			}
				
		}
		
	}

	UpdateStarField();
	UpdatePlayer();
	UpdateObjects();
	CheckCollisions();
	SpawnRandomObject();
	
}

static void render()
{
	RenderStarField();

	if (ship.isActive == 1)
	{
		BlitTexture(spaceShipTexture, ship.position.x, ship.position.y);
	}
		

	for (int i = 0; i < MAX_POOL_OBJECTS; i++)
	{
		if (objects[i].isActive==1)
		{

			BlitTexture(objects[i].texture, objects[i].position.x, objects[i].position.y);
		}
	}

	RenderHUD();

	if (gameOver)
	{
		DrawTextToCenter("GAME OVER");
	}
}

static void cleanup()
{
	printf("Exiting play state.\n");

	SDL_DestroyTexture(spaceShipTexture);
	SDL_DestroyTexture(junkTexture[0]);
	SDL_DestroyTexture(junkTexture[1]);
	SDL_DestroyTexture(junkTexture[2]);
	SDL_DestroyTexture(junkTexture[3]);
	SDL_DestroyTexture(projectileTexture);
	SDL_DestroyTexture(smokeTexture);
}

void UpdatePlayer()
{
	if (ship.isActive == 0)return;

	if (IsKeyDown(SDL_SCANCODE_LEFT))
	{
		ship.position.x -= ship.moveSpeed * deltaTime;
	}

	if (IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		ship.position.x += ship.moveSpeed * deltaTime;
	}

	if (IsKeyDown(SDL_SCANCODE_LCTRL))
	{
		
		FireProjectile();
	}

	if (ship.position.x < 0)
		ship.position.x = 0;

	if (ship.position.x + ship.w > SCREEN_WIDTH)
		ship.position.x = SCREEN_WIDTH - ship.w;
}
void UpdateObjects()
{
	for (int i = 0; i < MAX_POOL_OBJECTS; i++)
	{
		if (objects[i].isActive == 1)
		{
			objects[i].position.x += objects[i].velocity.x * deltaTime;
			objects[i].position.y += objects[i].velocity.y * deltaTime;

			if (objects[i].position.y > SCREEN_HEIGHT && objects[i].velocity.y>0 )
				objects[i].isActive = 0;

			if (objects[i].position.y < 0 && objects[i].velocity.y < 0)
				objects[i].isActive = 0;
		}
	}
}

void SpawnRandomObject()
{
	if ((SDL_GetTicks() - lastSpawnTime) / 1000 > spawnInterval)
	{
		
		int randNum = rand() % 6; //TODO: Maybe increase the number as the game progresses.

		for(int i=0; i < randNum; i++)
		{
			
			objects[oIndex].isActive = 1;
			objects[oIndex].position.x = rand() % SCREEN_WIDTH;
			objects[oIndex].position.y = -10 - (rand()%60);
			objects[oIndex].velocity.y = junkSpeed;
			
			objects[oIndex].tag = "junk";

			int junkType = rand() % 6;

			switch (junkType)
			{
			case 0:
				objects[oIndex].texture = junkTexture[0];
				objects[oIndex].velocity.y = junkSpeed * 1.5;
				break;
			case 1:
				objects[oIndex].texture = junkTexture[1];
				break;
			case 2:
				objects[oIndex].texture = junkTexture[2];
				objects[oIndex].velocity.y = junkSpeed * 0.5;
				break;
			case 3:
				objects[oIndex].texture = junkTexture[3];
				objects[oIndex].velocity.y = junkSpeed * 0.5;
				break;
			case 4:
				objects[oIndex].texture = junkTexture[4];
				break;
			case 5:
				objects[oIndex].texture = junkTexture[5];
				break;
			}

			SDL_QueryTexture(objects[oIndex].texture, NULL, NULL, &objects[oIndex].w, &objects[oIndex].w);
			

			oIndex++;
			if (oIndex >= MAX_POOL_OBJECTS)
				oIndex = 0;

			
		}

		lastSpawnTime = SDL_GetTicks();
	}
	

}

void FireProjectile()
{
	if ((SDL_GetTicks() - lastFiredTime)/1000 < fireRate)return;

	lastFiredTime = SDL_GetTicks();

	objects[oIndex].position.x = ship.position.x + ship.w / 2;
	objects[oIndex].position.y = ship.position.y;
	objects[oIndex].velocity.x = 0.0;
	objects[oIndex].velocity.y = -projectileSpeed; 
	objects[oIndex].texture = projectileTexture;
	objects[oIndex].tag = "bullet";
	objects[oIndex].isActive = 1;
	if (++oIndex >= MAX_POOL_OBJECTS)
		oIndex = 0;

	PlaySound(0, 0);
}

void CreateExplosion()
{
	int num = 6;
	int i;
	for (i = 0; i < num; i++)
	{
		objects[oIndex].position.x = ship.position.x + ship.w / 2;
		objects[oIndex].position.y = ship.position.y + ship.h / 2;
		objects[oIndex].velocity.x = (rand()%64)-(rand() % 64);
		objects[oIndex].velocity.y = (rand() % 64) - (rand() % 64);
		objects[oIndex].texture = smokeTexture;
		objects[oIndex].isActive = 1;
		objects[oIndex].tag = "smoke";
		if (++oIndex >= MAX_POOL_OBJECTS)oIndex = 0;
	}

	PlaySound(1, 1);
}

void RenderHUD()
{
	int padding = 40;

	char scoreChar[8];
	char text[55] = "SCORE: ";

	sprintf(scoreChar, "%d", playerScore);

	strcat(text, scoreChar);

	DrawText(text, padding , padding);
	
	int lives;
	int sWidth = ship.w * 0.33;
	int sHeight = ship.h * 0.33;

	for (lives = 0; lives < playerLives; lives++)
	{
		SDL_Rect src = { 0,0,ship.w,ship.h };
		SDL_Rect dest = { SCREEN_WIDTH - (lives * sWidth + padding), padding, sWidth, sHeight };
		SDL_RenderCopy(renderer, spaceShipTexture, &src, &dest);
	}

	if (paused)
	{
		DrawTextToCenter("PAUSED");
	}
		
}

void CheckCollisions()
{
	int i, j;
	
	for (i = 0; i < MAX_POOL_OBJECTS; i++)
	{
		if (objects[i].isActive == 0)continue;

		SDL_Rect junkRect = { objects[i].position.x,objects[i].position.y,objects[i].w,objects[i].h };
		SDL_Rect shipRect = { ship.position.x,ship.position.y,ship.w,ship.h };

		//Check ship and junk collisions
		if (ship.isActive == 1 && strcmp("junk",objects[i].tag) == 0 && CollidedRect(shipRect, junkRect))
		{
			objects[i].isActive = 0;
			playerLives--;
			ship.isActive = 0;
			CreateExplosion();
			continue;
		}
		
		//Check Projectile Collisions
		for (j = 0; j < MAX_POOL_OBJECTS; j++)
		{
			if (objects[j].isActive == 0 || strcmp(objects[j].tag,"bullet")!=0 || strcmp(objects[i].tag, "junk") != 0)continue;

			SDL_Rect projectileRect = { objects[j].position.x,objects[j].position.y, 14,31};

			if (CollidedRect(projectileRect, junkRect))
			{
				objects[i].isActive = 0;
				PlaySound(1, 1);
				playerScore++;
				if (playerScore % 50 == 0) //This should go before the bonus so its not skipped
				{
					playerLives++;
					PlaySound(2, 2);
				}
				if (objects[i].texture == junkTexture[0]) //give a little bonus
					playerScore += 9;
				
			}
		}
	}
}

GameState* GetPlayState(void)
{
	GameState* newState = (GameState*) malloc(sizeof(GameState));
	if (newState == NULL)
	{
		printf("Could not allocate memory for game state.\n");
		exit(1);
	}
	newState->start = start;
	newState->update = update;
	newState->render = render;
	newState->cleanup = cleanup;

	return newState;
}