#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>

void LoadFontTexture(char* filePath);
void BlitTexture(SDL_Texture* texture, int x, int y);
void DrawText(char* text, int x, int y);
void DrawColorText(char* text, int x, int y, int r, int g, int b);
void DrawTextToCenter(char* text);

#endif