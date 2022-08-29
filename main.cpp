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
#include "conexion.cpp"
#include "fachadaCharacters.h"
void charGenerator(unique_ptr<fachadaCharacters>&fachada, vector<character*>& tempCh, vector<character*>& tempEn, size_t x, size_t y, tower& torreAmiga, tower& torreEnemiga)
{
	fachada->sortingNgenerating(tempCh, tempEn, x, y, torreAmiga, torreEnemiga);
}
vector<int>DBtoInt(string s, string delimiter)
{
	vector<int> prueba;
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != string::npos)
	{
		string token = s.substr(0, pos);
		int tempNum = stoi(token);
		prueba.push_back(tempNum);
		s.erase(0, pos + delimiter.length());
	} {int tempNum = stoi(s); prueba.push_back(tempNum); }
	return prueba;
}
vector<string> DBtoString(string s, string delimiter)
{
	vector<string> prueba;
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != string::npos)
	{
		string token = s.substr(0, pos);
		prueba.push_back(token);
		s.erase(0, pos + delimiter.length());
	} {prueba.push_back(s); }
	return prueba;
}
void nothing(int& ventana, int cambio)
{
	ventana = cambio;
}
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
	if (fontMoney.getMoney(isMine) < SingletonData::getDB(ID, "price"))
		return;
	if (quantity < 50)
	{
		fontMoney.modifyMoney(SingletonData::getDB(ID, "price"), isMine, true);
		team.push_back(NULL);
		assignType(team[quantity], SingletonData::getDB(ID, "type"), ID, isMine);
		quantity++;

	}
	else if (quantity == 50)
	{
		for (int i = 0; i < 50; i++)
		{
			if (team[i] == NULL)
			{
				fontMoney.modifyMoney(SingletonData::getDB(ID, "price"), isMine, true);
				assignType(team[i], SingletonData::getDB(ID, "type"), ID, isMine);
				return;
			}
		}
	}
}
void saveAllData(int gameMode, vector<character*>aliados, vector<character*>enemies, background fondo, RemoteControl*controles, tower torreAmiga, tower torreEnemiga)
{
	vector<string> allData;
	allData.push_back("");
	for (size_t i{ 0 }; i < aliados.size(); i++)
	{
		if (aliados[i] != NULL)
		{
			allData[0] += aliados[i]->returnDataActual();
			if (i != aliados.size() - 1)
				allData[0] += "-";
		}
	}
	allData.push_back("");
	for (size_t i{ 0 }; i < enemies.size(); i++)
	{
		if (enemies[i] != NULL)
		{
			allData[1] += enemies[i]->returnDataActual();
			if (i != enemies.size() - 1)
				allData[1] += "-";
		}
	}
	allData.push_back(fondo.saveData());
	allData.push_back("");
	for (int i{ 0 }; i < 6; i++)
	{
		allData[3] += controles[i].saveData();
		if (i != 3)
			allData[3] += "-";
	}
	allData.push_back(torreAmiga.saveData(torreEnemiga));
	for (auto& i : allData)
	{
		cout << i << endl;
	}
	setDataBase(gameMode, allData);
}
void voidDataBase(int ID)
{
	vector<string> allData{ "0","0","0","0","0" };
	setDataBase(ID, allData);
}
void restoreData(int ID, vector<character*>&aliados, vector<character*>&enemies, size_t&x, size_t&y, background &fondo, RemoteControl* &controles, tower &torreAmiga, tower &torreEnemiga)
{
	vector<string> DB = getDataBase(ID);
	bool allZero = true;
	for (auto& i : DB)
	{
		if (i != "0")
			allZero = false;
	}
	if (allZero) { return; }
	vector<string> charactersData = DBtoString(DB[0], "-");
	if (charactersData[0].size() != 0)
	{
		x += charactersData.size();
		for (size_t i{ 0 }; i < x; i++)
		{
			aliados.push_back(NULL);
			int IDChar = (int)charactersData[i][0];
			IDChar -= 48;
			assignType(aliados[i], SingletonData::getDB(IDChar, "type"), IDChar, true);
			aliados[i]->setDataActual(DBtoInt, charactersData[i]);
		}
	}

	vector<string> enemiesData = DBtoString(DB[1], "-");
	
	if (enemiesData[0].size() != 0)
	{
		y += enemiesData.size();
		for (size_t i{ 0 }; i < y; i++)
		{
			enemies.push_back(NULL);
			int IDChar = (int)enemiesData[i][0];
			IDChar -= 48;
			assignType(enemies[i], SingletonData::getDB(IDChar, "type"), IDChar, false);
			enemies[i]->setDataActual(DBtoInt, enemiesData[i]);
		}
	}
	cout << "hola"<< endl;
	fondo.setData(DBtoInt, DB[2]);
	for (int i{ 0 }; i < 6; i++)
		controles[i].setData(DBtoInt, DB[3]);
	torreAmiga.setData(DBtoInt, DB[4]);
	torreEnemiga.setData(DBtoInt, DB[4]);
}
void resetAll(tower& torreAmiga, tower& torreEnemiga, background& fondo, vector<character*>& aliados, vector<character*>& enemies, size_t& x, size_t& y)
{
	torreAmiga.setCheckLife(torreAmiga.getCheckLifeConst());
	torreEnemiga.setCheckLife(torreEnemiga.getCheckLifeConst());
	fondo.resetMoney();
	while (aliados.size() != 0)
		aliados.pop_back();
	while (enemies.size() != 0)
		enemies.pop_back();
	x = 0;
	y = 0;
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
	SingletonData::getInstance();
	tower torreAmiga(100, true, 1);
	tower torreEnemiga(100, false, 1);
	RemoteControl* control = new RemoteControl[10]
	{
		RemoteControl(new generateCommand("btn_1", 150, 100, generateCharacter, 120,"A")),
		RemoteControl(new generateCommand("btn_1", 1650, 100, generateCharacter, 120,"L")),
		RemoteControl(new generateCommand("btn_2", 300, 100, generateCharacter, 240,"S")),
		RemoteControl(new generateCommand("btn_2", 1500, 100, generateCharacter, 240,"K")),
		RemoteControl(new generateCommand("btn_3", 450, 100, generateCharacter, 600,"D")),
		RemoteControl(new generateCommand("btn_3", 1350, 100, generateCharacter, 600,"J")),
		RemoteControl(new voidCommand("btn_multijugador", 700, 360, nothing, 2, "1")),
		RemoteControl(new voidCommand("btn_solitario",700,535,nothing,2, "2")),
		RemoteControl(new voidCommand("btn_salir",700,680,nothing,2, "ESCAPE")),
		RemoteControl(new voidCommand("btn_salir",700,720,nothing,2, "BACKSPACE"))
	};
	background* fondos = new background[5];
	for (int i = 0; i < 5; i++)
		i > 1 ? fondos[i].setValores(i + 1, 0) : fondos[i].setValores(i + 1, 2 - i);
	
	size_t x = 0;
	size_t y = 0;
	int x_ax = 0;
	int y_ax = 0;
	bool running = true;
	al_start_timer(timer);
	bool winner{false};
	int ventana{1};
	
	unique_ptr<fachadaCharacters> sortingChar = make_unique<fachadaCharacters>();
	restoreData(1, aliados, enemies, x, y, fondos[1], control, torreAmiga, torreEnemiga);
	restoreData(2, aliados, enemies, x, y, fondos[0], control, torreAmiga, torreEnemiga);

	while (running)
	{
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			running = false;
		x = aliados.size();
		y = enemies.size();
		switch (ventana)
		{
			case 1:
			{
				for (int i{ 6 }; i < 9; i++)
					control[i].recharBtn(event, keyState);
				al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
				fondos[2].generateBG();
				control[6].executeButton(ventana,2);		
				control[7].executeButton(ventana, 3);
				control[8].executeButton(ventana, 6);
			}break;
			case 2:
			{
				for (int i{ 0 }; i < 6; i++)
					control[i].recharBtn(event, keyState);				
				if (event.type == ALLEGRO_EVENT_TIMER)
				{
					al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
					fondos[1].generateBG();
					torreAmiga.generateTower();
					torreEnemiga.generateTower();
					control[0].executeButton(1, aliados, x, true, fondos[1], false);
					control[1].executeButton(1, enemies, y, false, fondos[1], false);
					control[2].executeButton(2, aliados, x, true, fondos[1], false);
					control[3].executeButton(2, enemies, y, false, fondos[1], false);
					control[4].executeButton(3, aliados, x, true, fondos[1], false);
					control[5].executeButton(3, enemies, y, false, fondos[1], false);

					charGenerator(sortingChar, aliados, enemies, x, y, torreAmiga, torreEnemiga);

					if (torreAmiga.getCheckLife() <= 0) {
						ventana = 4;
						winner = false;
						resetAll(torreAmiga, torreEnemiga, fondos[1], aliados, enemies,x,y);
						voidDataBase(1);
					}
					if (torreEnemiga.getCheckLife() <= 0)
					{
						ventana = 4;
						winner = true;
						resetAll(torreAmiga, torreEnemiga, fondos[1], aliados, enemies,x,y);
						voidDataBase(1);
					}
				}				
			}break;
			case 3:
			{
				control[0].recharBtn(event, keyState);
				control[2].recharBtn(event, keyState);
				control[4].recharBtn(event, keyState);
				if (event.type == ALLEGRO_EVENT_TIMER)
				{
					al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
					fondos[0].generateBG();
					torreAmiga.generateTower();
					torreEnemiga.generateTower();
					control[0].executeButton(1, aliados, x, true, fondos[0], false);
					control[1].executeButton(1, enemies, y, false, fondos[0], true);
					control[2].executeButton(2, aliados, x, true, fondos[0], false);
					control[3].executeButton(2, enemies, y, false, fondos[0], true);
					control[4].executeButton(3, aliados, x, true, fondos[0], false);
					control[5].executeButton(3, enemies, y, false, fondos[0], true);

					charGenerator(sortingChar, aliados, enemies, x, y, torreAmiga, torreEnemiga);

					if (torreAmiga.getCheckLife() <= 0) {
						ventana = 4;
						winner = false;
						resetAll(torreAmiga, torreEnemiga, fondos[0], aliados, enemies, x, y);
						voidDataBase(1);
					}
					if (torreEnemiga.getCheckLife() <= 0) {
						ventana = 4;
						winner = false;
						resetAll(torreAmiga, torreEnemiga, fondos[0], aliados, enemies, x, y);
						voidDataBase(1);
					}

				}
			}break;
			case 4:
			{
				control[9].recharBtn(event, keyState);
				if (event.type == ALLEGRO_EVENT_TIMER)
				{
					if (winner)
						fondos[3].generateBG();
					else
						fondos[4].generateBG();
					control[9].executeButton(ventana, 1);
				}
			}break;
			case 6:
			{
				running = false;
			}break;
		}
		al_flip_display();
	}
	if (ventana == 2)
		saveAllData(1, aliados, enemies, fondos[1], control, torreAmiga, torreEnemiga);
	if (ventana == 3)
		saveAllData(2, aliados, enemies, fondos[0], control, torreAmiga, torreEnemiga); cout << "hola";
	SingletonData::deleteData();
	delete[] control;
	delete[] fondos;  
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_sample(sample);
	al_destroy_sample_instance(sampleInstance);
	al_uninstall_audio();
	al_uninstall_mouse();
	al_destroy_timer(timer);

}