#pragma once
#include <iostream>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
using namespace std;
class sound
{
	protected:
		ALLEGRO_SAMPLE* sample;
	public:
		sound(string soundName)
		{
			const char* name = soundName.c_str();
			sample = al_load_sample(name);
		}
		~sound(){al_destroy_sample(sample);}
		void playSound(){al_play_sample(sample, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);}
};

