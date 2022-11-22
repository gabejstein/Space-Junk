#ifndef SOUND_H
#define SOUND_H

#include "game.h"

void InitSound(void);
void PlaySound(int id, int channel);
void PlayMusic(int loop);

#endif