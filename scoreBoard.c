#include "scoreBoard.h"

Score highScores[MAX_HIGH_SCORES];

Vec2 tableOffset = {240,200};
int spacing = 50;

char* saveFilePath = "Data/scores.sav";

int latestScore = 0;
char newName[MAX_NAME_LENGTH];

static void SortScores(Score s[]);
static int LoadScores(char* filePath);
static int SaveScores(char* filePath);
static void renderScoreBoard(void);
static void renderNameSelect(void);
static void UpdateNameInput(void);

static int LoadScores(char* filePath)
{
	char fileFormatter[] =  "%[^,],%d\n";

	FILE* f = fopen(filePath, "r");
	if (f == NULL) { return 0; }

	int i, read;
	for (i = 0; i < MAX_HIGH_SCORES; i++)
	{
		read = fscanf(f, fileFormatter, highScores[i].name, &highScores[i].score);
		if (read < 2) { return 0; }
	}
	fclose(f);

	return 1;
}

static int SaveScores(char* filePath)
{
	char fileFormatter[] = "%s,%d\n";

	FILE* f = fopen(filePath, "w");
	if (f == NULL) {  return 0; }

	int i;
	for (i = 0; i < MAX_HIGH_SCORES; i++)
	{
		fprintf(f, fileFormatter,highScores[i].name,highScores[i].score);
	}

	fclose(f);

	return 1;
}

void InitScoreBoard(void)
{
	if (LoadScores(saveFilePath) == 0)
	{
		printf("Could not load score file.\n");
		//Generate default scoreboard if file could not be loaded.
		int i;
		for (i = 0; i < MAX_HIGH_SCORES; i++)
		{
			strncpy(highScores[i].name, "NONE", MAX_NAME_LENGTH);
			highScores[i].score = 500 - i * 10;
		}
	}
}

static void start()
{
	int i;
	for (i = 0; i < MAX_HIGH_SCORES; i++)
		highScores[i].isRecent = 0;


	ResetKey(SDL_SCANCODE_RETURN);

}

static void update()
{
	if (latestScore > highScores[MAX_HIGH_SCORES - 1].score)
	{
		UpdateNameInput();
	}
	else
	{
		if (IsKeyDown(SDL_SCANCODE_RETURN))
		{
			ChangeStates(GetTitleState());
		}
	}
	

	UpdateStarField();
}

static void render()
{
	RenderStarField();

	if (latestScore > highScores[MAX_HIGH_SCORES - 1].score)
	{
		renderNameSelect();
	}
	else
	{
		renderScoreBoard();
	}
	
}

static void renderNameSelect(void)
{
	int horOffset = 50;

	DrawTextToCenter("ENTER YOUR NAME:");

	if ((SDL_GetTicks() / 500) % 2)
	{
		//TODO: Draw blinking rectangle below text.
		SDL_Rect r;
		r.x = (SCREEN_WIDTH / 2- horOffset) + (strlen(newName) * GLYPH_WIDTH);
		r.y = SCREEN_HEIGHT / 2 + 50;
		r.w = GLYPH_WIDTH;
		r.h = GLYPH_HEIGHT;
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &r);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	
	//TODO: Draw name text.
	DrawText(newName, SCREEN_WIDTH / 2-horOffset, SCREEN_HEIGHT / 2 + 50);
}

static void renderScoreBoard(void)
{
	int i;

	for (i = 0; i < MAX_HIGH_SCORES; i++)
	{
		char buffer[256];
		sprintf(buffer, "%d.)%s.....%d", (i + 1), highScores[i].name, highScores[i].score);
		if (highScores[i].isRecent)
		{
			DrawColorText(buffer, tableOffset.x, tableOffset.y + i * spacing, 0,255,0);
		}
		else
		{
			DrawText(buffer, tableOffset.x, tableOffset.y + i * spacing);
		}
		

	}
}

static void cleanup()
{
	ResetKey(SDL_SCANCODE_RETURN);
}

static void UpdateNameInput(void)
{
	//Set name based on input.
	int i, n;
	char c;

	n = strlen(newName);

	for (i = 0; i < strlen(textInputBuffer); i++)
	{
		c = toupper(textInputBuffer[i]);

		if (n < MAX_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
		{
			newName[n++] = c;
		}
	}

	if (n > 0 && IsKeyDown(SDL_SCANCODE_BACKSPACE))
	{
		newName[--n] = '\0';

		ResetKey(SDL_SCANCODE_BACKSPACE);
	}


	//If enter is pressed, add the new score.
	if (IsKeyDown(SDL_SCANCODE_RETURN))
	{
		if (strlen(newName) == 0)
			strncpy(newName, "NONE", MAX_NAME_LENGTH);
		AddNewScore();
		ResetKey(SDL_SCANCODE_RETURN);
	}
		
}

void SetLatestScore(int newScore)
{
	latestScore = newScore;
	memset(newName, '\0', sizeof(newName));
}

void AddNewScore()
{
	
	//Make new array one item bigger and copy the high scores to it.
	Score tempArray[MAX_HIGH_SCORES+1];

	int i;
	for (i = 0; i < MAX_HIGH_SCORES; i++)
		tempArray[i] = highScores[i];

	tempArray[MAX_HIGH_SCORES].score = latestScore;
	strncpy(tempArray[MAX_HIGH_SCORES].name, newName,MAX_NAME_LENGTH);
	tempArray[MAX_HIGH_SCORES].isRecent = 1;


	//Sort it.
	SortScores(tempArray);

	//Copy the new array to the original.
	for (i = 0; i < MAX_HIGH_SCORES; i++)
		highScores[i] = tempArray[i];

	latestScore = 0;

	//Save the file.
	SaveScores(saveFilePath);
}

static void SortScores(Score s[])
{
	int i, n;
	for (n = MAX_HIGH_SCORES; n > 0; n--)
	{
		for (i = 0;i < n; i++)
		{
			if (s[i].score < s[i + 1].score)
			{
				Score temp = s[i + 1];
				s[i + 1] = s[i];
				s[i] = temp;
			}
		}
	}
}

GameState* GetScoreBoardState(void)
{
	GameState* newState = (GameState*)malloc(sizeof(GameState));
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

