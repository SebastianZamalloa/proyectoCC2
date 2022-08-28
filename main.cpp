#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include "shortRange.h"
#include "background.h"
#include "tower.h"
#include "button.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
using namespace std;
size_t x = 0;
size_t y = 0;
void assignType(character*& unit, int type, int ID, bool isMine)
{
	switch (type)
	{
	case 1:
		unit = new shortRange(ID, isMine);
		break;
	default:
		unit = new character(ID, isMine);
		break;
	}
}

void generateCharacter(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney)
{
	if (fontMoney.getMoney(isMine) < getDB(ID, "price"))
		return;
	if (quantity < 50)
	{
		fontMoney.modifyMoney(getDB(ID, "price"), isMine, true);
		team.push_back(NULL);
		assignType(team[quantity], getDB(ID, "type"), ID, isMine);
		quantity++;
	}
	else if (quantity == 50)
	{
		for (int i = 0; i < 50; i++)
		{
			if (team[i] == NULL)
			{
				fontMoney.modifyMoney(getDB(ID, "price"), isMine, true);
				assignType(team[i], getDB(ID, "type"), ID, isMine);
				return;
			}
		}
	}
}

int main()
{
	ALLEGRO_DISPLAY* display;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT event;
	ALLEGRO_SAMPLE* sample = NULL;
	ALLEGRO_SAMPLE_INSTANCE* sampleInstance = NULL;
	srand(time(NULL));
	al_init();
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

	display = al_create_display(1920, 1080);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	//al_set_window_position(display, 0, 0);
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();

	al_reserve_samples(3);
	sample = al_load_sample("music.wav");
	sampleInstance = al_create_sample_instance(sample);
	al_set_sample_instance_playmode(sampleInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(sampleInstance, al_get_default_mixer());
	al_play_sample_instance(sampleInstance);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_init_image_addon();
	vector<character*>aliados;
	vector<character*>enemies;
	dataIntialize();
	tower torreAmiga(100, true, 1);
	tower torreEnemiga(100, false, 1);
	background prueba(2);
	commandI* botones[3];
	RemoteControl* controles[3];
	botones[0] = new generateCommand("btn_1", 150, 100, generateCharacter, 120);
	botones[1] = new generateCommand("btn_1", 1650, 100, generateCharacter, 120);
	botones[2] = new generateCommand("btn_1", 300, 100, generateCharacter, 240);
	controles[0] = new RemoteControl(botones[0]);
	controles[1] = new RemoteControl(botones[1]);
	controles[2] = new RemoteControl(botones[2]);

	bool running = true;
	int mousex = 0;
	int mousey = 0;
	al_start_timer(timer);

	bool winner;
	int ventana = 1;

	while (running)
	{
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			running = false;
		controles[0]->recharBtn(event);
		controles[1]->recharBtn(event);
		controles[2]->recharBtn(event);
		x = aliados.size();
		y = enemies.size();

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
			prueba.generateBG();
			torreAmiga.generateTower();
			torreEnemiga.generateTower();
			controles[0]->executeButton(1, aliados, x, true, prueba, false);
			controles[1]->executeButton(1, enemies, y, false, prueba, false);
			controles[2]->executeButton(2, aliados, x, true, prueba, false);
			for (int i = 0; i < x; i++)
			{
				vector<character*>aliadostemp = aliados;
				sort(aliadostemp.begin(), aliadostemp.end(), comparator);
				if (aliadostemp[i] != NULL)
					aliadostemp[i]->generateCharacter(enemies, &torreEnemiga);
			}
			for (int i = 0; i < y; i++)
			{
				vector<character*>enemigostemp = enemies;
				sort(enemigostemp.begin(), enemigostemp.end(), comparator);
				if (enemigostemp[i] != NULL)
					enemigostemp[i]->generateCharacter(aliados, &torreAmiga);
			}

			al_flip_display();
		}
	}

	deleteData();
	for (int i = 0; i < 3; i++)
		delete botones[i];
	for (int i = 0; i < 3; i++)
		delete controles[i];
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_sample(sample);
	al_destroy_sample_instance(sampleInstance);
	al_uninstall_audio();
	al_uninstall_mouse();
	al_destroy_timer(timer);

}