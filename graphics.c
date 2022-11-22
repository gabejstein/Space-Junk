#include "graphics.h"

SDL_Texture* fontTexture;

void LoadFontTexture(char* filePath)
{
	SDL_Surface* tempSurface;
	tempSurface = IMG_Load(filePath);

	if (tempSurface == NULL)
	{
		printf("Could not load font texture.\n");
		exit(1);
	}

	fontTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void BlitTexture(SDL_Texture* texture, int x, int y)
{
	SDL_Rect src;
	SDL_Rect dest;

	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
	src.x = 0;
	src.y = 0;
	dest.x = x;
	dest.y = y;
	dest.w = src.w;
	dest.h = src.h;

	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void DrawText(char* text, int x, int y)
{
	int length = strlen(text);

	SDL_Rect src, dest;

	for (int i = 0; i < length; i++)
	{
		src.x = GLYPH_WIDTH * (text[i] - 32);
		src.y = 0;
		src.w = GLYPH_WIDTH;
		src.h = GLYPH_HEIGHT;
		dest.x = x + i * GLYPH_WIDTH;
		dest.y = y;
		dest.w = src.w;
		dest.h = src.h;

		SDL_RenderCopy(renderer, fontTexture, &src, &dest);
	}
}

void DrawColorText(char* text, int x, int y, int r, int g, int b)
{
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	DrawText(text, x, y);
	SDL_SetTextureColorMod(fontTexture, 255, 255, 255);
}

void DrawTextToCenter(char* text)
{
	int length = strlen(text);
	int x = (SCREEN_WIDTH/2) - (length * GLYPH_WIDTH / 2);
	int y = (SCREEN_HEIGHT/2)  - (GLYPH_HEIGHT / 2);
	DrawText(text, x, y);
}