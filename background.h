#pragma once
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "image.h"
#include <string>
using namespace std;

class background
{
	protected:
		int numBG;
		image imageBG;
		ALLEGRO_FONT* moneyText;
		int moneyVar[2];
		int moneyLimit;
		int counter;
	public:
		background(int numTemp) :numBG(numTemp)
		{
			moneyText = al_load_ttf_font("wantCoffee.ttf", 32, 0);
			string nameBG = to_string(numTemp);
			nameBG = "background/" + nameBG;
			imageBG.setBitmap(nameBG, ".png");
			moneyVar[0] = 0; moneyVar[1] = 0;
			moneyLimit = 1000;
			counter = 0;
		}
		~background() { al_destroy_font(moneyText); }
		ALLEGRO_BITMAP* getBG() { return imageBG.getBitmap(); }
		void generateBG()
		{
			imageBG.generateImage(0, 0);
			string money1 = to_string(moneyVar[0]);
			string money2 = to_string(moneyVar[1]);
			const char* moneyChar1 = money1.c_str();
			const char* moneyChar2 = money2.c_str();
			al_draw_text(moneyText, al_map_rgb(255, 255, 255), 10, 10, 0, moneyChar1);
			al_draw_text(moneyText, al_map_rgb(255, 255, 255), 1850, 10, 0, moneyChar2);
			counter++;
			if (counter == 1)
			{
				if (moneyVar[0] < moneyLimit) moneyVar[0] += 1;
				if (moneyVar[1] < moneyLimit) moneyVar[1] += 1;
				counter = 0;
			}
		}
		void modifyMoney(int cost, bool isMine, bool isCost)
		{
			if (isCost) { if (isMine) { moneyVar[0] -= cost; } else { moneyVar[1] -= cost; } }
			else { if (isMine) { moneyVar[0] += cost; } else { moneyVar[1] += cost; } }
		}
		int getMoney(bool isMine)
		{
			if (isMine) { return moneyVar[0]; }
			else { return moneyVar[1]; }
		}
		void resetMoney() {
			moneyVar[0] = 0;
			moneyVar[1] = 0;
		}
};