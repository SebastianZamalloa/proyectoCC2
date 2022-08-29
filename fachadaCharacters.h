#pragma once
#include "shortRange.h"
#include "iostream"
#include <algorithm>
using namespace std;
class fachadaCharacters
{
protected:
	vector<character*>tempCharacter;
	vector<character*>tempEnemie;
public:
	fachadaCharacters() {}
	fachadaCharacters(vector<character*>tempCh, vector<character*>tempEn) :tempCharacter(tempCh), tempEnemie(tempEn) {}
	~fachadaCharacters() {}
	void setFachada(vector<character*>tempCh, vector<character*>tempEn)
	{
		tempCharacter = tempCh;
		tempEnemie = tempEn;
	}
	void sortingNgenerating(vector<character*>&tempCh, vector<character*>&tempEn,size_t x, size_t y, tower& torreAmiga, tower& torreEnemiga)
	{
		for (int i = 0; i < x; i++)
		{
			setFachada(tempCh, tempEn);
			sort(tempCharacter.begin(), tempCharacter.end(), comparator);
			if (tempCharacter[i] != NULL)
				tempCharacter[i]->generateCharacter(tempEn, &torreEnemiga);
		}
		for (int i = 0; i < y; i++)
		{
			setFachada(tempCh, tempEn);
			sort(tempEnemie.begin(), tempEnemie.end(), comparator);
			if (tempEnemie[i] != NULL)
				tempEnemie[i]->generateCharacter(tempCh, &torreAmiga);
		}
	}
};