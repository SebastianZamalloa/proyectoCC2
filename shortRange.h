#pragma once
#include "character.h"
#include <iostream>
using namespace std;

class shortRange : public character
{
private:
    int splashNumber;
    int objetiveRange;
public:
    shortRange(int auxID, bool propery) :character(auxID, propery)
    {
        splashNumber = SingletonData::getDB(ID, "extraData1");
        objetiveRange = SingletonData::getDB(ID, "extraData2");
    }
    ~shortRange() {}
    vector<int> getEnemies(vector<character*> enemies)
    {
        vector<int> enemiesID;
        if (isMine)
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i] != NULL)
                    if (posX + hitboXEnd + range <= enemies[i]->getPosX() + 100 + objetiveRange && posX + hitboXEnd + range >= enemies[i]->getPosX() )
                    {
                        if (enemiesID.size() == splashNumber)
                            return enemiesID;
                        else
                            enemiesID.push_back(i);
                    }
            }
        }
        else
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i] != NULL)
                    if (posX + hitboXOrigin - range >= enemies[i]->getPosX() + enemies[i]->getHitboxEnd() - 100 - objetiveRange && posX + hitboXOrigin - range <= enemies[i]->getPosX() + enemies[i]->getHitboxEnd())
                    {
                        if (enemiesID.size() == splashNumber)
                            return enemiesID;
                        enemiesID.push_back(i);
                    }
            }
        }
        return enemiesID;
    }
    void generateCharacter(vector<character*>& enemies, tower* enemieTower)
    {
        bool haveTower = detect_tower(enemieTower);
        bool haveEnemy = false;

        if (haveTower)
        {
            animateCharacter(3);
            atacar(enemies, enemieTower, true);
        }
        else if (!haveTower)
        {
            if (focusID >= 0)
                if (enemies[focusID] == NULL)
                    focusID = -1;

            haveEnemy = detect_enemie(enemies);
            if (haveEnemy)
            {
                if (areaDamage)
                {
                    vector<int> enemiesID = getEnemies(enemies);
                    animateCharacter(3);
                    atacar(enemies, enemieTower, false, enemiesID);

                }
                else
                {
                    animateCharacter(3);
                    atacar(enemies, enemieTower, false);
                }

            }
            else if (animationState[2])
            {
                animateCharacter(3);
            }
            else
                animateCharacter(2);
        }
    }
    void atacar(vector<character*>& enemies, tower* enemieTower, bool isTower)
    {
        if (frameTravel == frameAttack - 1 && frameCounter == 1)
        {
            soundAttack[rand() % (2)].playSound();
            if (isTower)
            {
                enemieTower->modifyHealth(damage, true);
                if (enemieTower->getHealth() <= 0)
                {
                    cout << "GANASTE" << endl;
                }
            }
            else
            {
                enemies[focusID]->modifyHP(damage);
                if (enemies[focusID]->getHP() <= 0)
                {
                    delete enemies[focusID];
                    enemies[focusID] = NULL;
                    focusID = -1;
                }
            }
        }
    }
    void atacar(vector<character*>& enemies, tower* enemieTower, bool isTower, vector<int>enemiesID)
    {
        if (frameTravel == frameAttack - 1 && frameCounter == 1)
        {
            soundAttack[rand() % (2)].playSound();
            if (isTower)
            {
                enemieTower->modifyHealth(damage, true);
                if (enemieTower->getHealth() <= 0)
                {
                    cout << "GANASTE" << endl;
                }
            }
            else
            {
                bool haveTower = detect_tower(enemieTower);
                if (haveTower)
                {
                    enemieTower->modifyHealth(damage, true);
                    if (enemieTower->getHealth() <= 0)
                    {
                        cout << "GANASTE" << endl;
                    }
                }
                for (int i = 0; i < enemiesID.size(); i++)
                {
                    enemies[enemiesID[i]]->modifyHP(damage);                
                    if (enemies[enemiesID[i]]->getHP() <= 0)
                    {
                        delete enemies[enemiesID[i]];
                        enemies[enemiesID[i]] = NULL;
                        focusID = -1;
                    }
                }
            }
        }
    }
};