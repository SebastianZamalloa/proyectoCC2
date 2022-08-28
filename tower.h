#pragma once
#include <iostream>
#include <string>
#include "image.h"
#include "allegro5/allegro_primitives.h"
using namespace std;

class tower
{
protected:
	bool isMine;
	int constHealth;
	int varHealth;
	int towerID;
	int towerX;
	image towerBitmap;
public:
	tower(int auxHealth, bool propery, int num) :constHealth(auxHealth), isMine(propery), towerID(num), varHealth(auxHealth)
	{
		string nameTower = to_string(towerID);
		if (!isMine)
			nameTower = "enemy/" + nameTower;
		nameTower = "tower/" + nameTower;
		towerBitmap.setBitmap(nameTower, ".png");
		towerX = towerBitmap.getWidth() / 2;
		if (isMine) {towerX += 100;}
		else { towerX += 1820 - towerBitmap.getWidth(); }
	}
	tower() :constHealth(999999), isMine(true), towerID(0), towerX(0), varHealth(0) {}
	int getHealth() { return varHealth; }
	int getTowerX() { return towerX; }
	void modifyHealth(int damage, bool isDamage)
	{
		if (isDamage) { varHealth -= damage; }
		else { varHealth += damage; }
	}
	void generateTower()
	{
		
		int x = 100;
		int y = 650 - towerBitmap.getHeight();
		if (!isMine) { x = 1820 - towerBitmap.getWidth(); }
		towerBitmap.generateImage(x, y);
		x = 150;
		if (!isMine) { x = 1870 - towerBitmap.getWidth(); }

		float percent;
		if (varHealth > 0)
			percent = varHealth * 100.0 / constHealth;
		else
			percent = 0;
		al_draw_filled_rectangle(x, 680, x + 130 , 700, al_map_rgb(255, 255, 255));
		al_draw_rectangle(x, 680, x + 130, 700, al_map_rgb(0, 0, 0),3);
		al_draw_filled_rectangle(x, 680, x + 130 * (percent / 100.0), 700, al_map_rgb(0,0,0));
	}
	int getCheckLife() {
		return varHealth;
	}
	int getCheckLifeConst() {
		return constHealth;
	}
	void setCheckLife(int life) {
		varHealth = life;
	}
};