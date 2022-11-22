#include "sound.h"


Mix_Music* music = NULL;
static Mix_Chunk* sounds[3];

void InitSound(void)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS("Assets/Sounds/music.ogg");
	sounds[0] = Mix_LoadWAV("Assets/Sounds/laser1.wav");
	sounds[1] = Mix_LoadWAV("Assets/Sounds/Explosion_03.wav");
	sounds[2] = Mix_LoadWAV("Assets/Sounds/Pickup_04.wav");
}

void PlaySound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}

void PlayMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}