#pragma once
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
using namespace std;

class image
{
protected:
	string archiveName;
	ALLEGRO_BITMAP* bitmap;
	float widthCh, heightCh;
public:
	image(string nameTemp, string type) { setBitmap(nameTemp, type); }
	image()
	{
		archiveName = "";
		bitmap = NULL;
		widthCh = heightCh = 0;
	}
	~image() { al_destroy_bitmap(bitmap); }
	void setBitmap(string nameTemp, string type)
	{
		archiveName = nameTemp + type;
		const char* name = archiveName.c_str();
		bitmap = al_load_bitmap(name);
		widthCh = al_get_bitmap_width(bitmap);
		heightCh = al_get_bitmap_height(bitmap);
	}
	ALLEGRO_BITMAP* getBitmap() { return bitmap; }
	void generateImage(int x, int y) { al_draw_bitmap(bitmap, x, y, 0); }
	void generateImage(int corx, int cory, int tamx, int tamy, int x, int y)
	{
		al_draw_bitmap_region(bitmap, corx, cory, tamx, tamy, x, y, 0);
	}
	float getWidth() { return widthCh; }
	float getHeight() { return heightCh; }
};