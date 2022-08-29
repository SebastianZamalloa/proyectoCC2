#pragma once
#include <iostream>
#include <cstdlib> 
#include <string>
#include "image.h"
#include "tower.h"
#include <vector>
#include "data.cpp"
#include <memory>
#include "sound.h"



class character
{
protected:
    int ID;
    int HP;
    int range;
    int damage;
    float attackSpeed;
    float speedMovement;
    bool areaDamage;
    int price;

    float posX;
    float posY;
    int addPosY;
    bool isMine;

    int sizeXSheet;
    int sizeYSheet;
    int posXSheet;
    int posYSheet;

    int hitboXOrigin;
    int hitboXEnd;

    int frameCountWalk;
    int frameCountAttack;
    int* frameDurationWalk;
    int* frameDurationAttack;
    int frameAttack;
    int focusID = -1;

    bool animationState[3]{ false,false,false };
    void verifyUniqueAnimation(int);

    int frameTravel;
    int frameCounter;

    image characterSheet;
    sound soundAttack[2] = { sound("audio/attack/1.mp3"),sound("audio/attack/2.mp3") };

public:
    character() { reInitialize(0, true); }
    character(int auxID, bool propery) { reInitialize(auxID, propery); }
    int getID() { return ID; }
    int getPosX() { return posX; }
    int getHP() { return HP; }
    int getHitboxEnd() { return hitboXEnd; }
    void reInitialize(int auxID, bool propery)
    {
        ID = auxID;
        isMine = propery;
        if (ID > 0)
        {
            string nameCharacter = to_string(ID);
            if (!isMine)
                nameCharacter = "enemy/" + nameCharacter;
            nameCharacter = "characters/" + nameCharacter;
            characterSheet.setBitmap(nameCharacter, ".png");
        }
        HP = getDB(ID, "HP");
        range = getDB(ID, "range");
        damage = getDB(ID, "damage");

        attackSpeed = getDB(ID, "attackSpeed", true);
        speedMovement = getDB(ID, "speedMovement", true);

        areaDamage = getDB(ID);
        price = getDB(ID, "price");

        sizeXSheet = getDB(ID, "sizeXSheet");
        sizeYSheet = getDB(ID, "sizeYSheet");
        frameCountWalk = getDB(ID, "frameCountWalk");
        frameDurationWalk = getDB(ID, 0);
        frameCountAttack = getDB(ID, "frameCountAttack");
        frameDurationAttack = getDB(ID, 1);
        hitboXOrigin = getDB(ID, "hitboXOrigin");
        hitboXEnd = getDB(ID, "hitboXEnd");
        frameTravel = 0;
        frameCounter = 1;
        frameAttack = getDB(ID, "frameAttack");

        posXSheet = 0;
        posYSheet = 0;
        posX = 50;
        if (!isMine) posX = 1870 - sizeXSheet;
        addPosY = (rand() % (8)) * 4;
        posY = 650 + addPosY;
    }
    //ID HP frameTravel frameCounter posXSheet posYSheet posX posY 
    virtual ~character() {}
    string returnDataActual()
    {
        string result;
        result += to_string(ID) + " ";
        result += to_string(HP) + " ";
        result += to_string(frameTravel) + " ";
        result += to_string(posXSheet) + " ";
        result += to_string(posYSheet) + " ";
        result += to_string((int)posX) + " ";
        result += to_string((int)posY);
        return result;
    }
    void setDataActual(vector<int>(*func)(string, string),string data)
    {
        vector<int> result = func(data," ");
        ID = result[0];
        HP = result[1];
        frameTravel = result[2];
        posXSheet = result[3];
        posYSheet = result[4];
        posX = (float)result[5];
        posY = (float)result[6];
    }
    int getAddPosY() { return addPosY; }
    void avanzar() { if (isMine) posX += 1 * speedMovement; else { posX -= 1 * speedMovement; } }
    virtual bool detect_tower(tower* enemieTower)
    {
        if (isMine)
        {
            if (posX + hitboXEnd + range <= enemieTower->getTowerX() + 1000 && posX + hitboXEnd + range >= enemieTower->getTowerX())
                return true;
        }
        else
        {
            if (posX + hitboXOrigin - range >= enemieTower->getTowerX() - 1000 && posX + hitboXOrigin - range <= enemieTower->getTowerX())
                return true;
        }
        return false;
    }
    virtual bool detect_enemie(vector<character*> enemies)
    {
        if (isMine)
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i] != NULL)
                    if (posX + hitboXEnd + range <= enemies[i]->getPosX() + 100 && posX + hitboXEnd + range >= enemies[i]->getPosX())
                    {
                        focusID = i;
                        return true;
                    }
            }
        }
        else
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i] != NULL)
                    if (posX + hitboXOrigin - range >= enemies[i]->getPosX() + enemies[i]->getHitboxEnd() - 100 && posX + hitboXOrigin - range <= enemies[i]->getPosX() + enemies[i]->getHitboxEnd())
                    {
                        focusID = i;
                        return true;
                    }
            }
        }
        return false;
    }
    virtual void atacar(vector<character*>& enemies, tower* enemieTower, bool isTower)
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
    virtual void generateCharacter(vector<character*>& enemies, tower* enemieTower)
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
                animateCharacter(3);
                atacar(enemies, enemieTower, false);
            }
            else if (animationState[2])
            {
                animateCharacter(3);
            }
            else
                animateCharacter(2);
        }
    }
    void modifyHP(int x) { HP -= x; }

    void animateCharacter(int state) //linea de tiempo
    {
        int postempY = posY - sizeYSheet;
        posYSheet = (state - 1) * sizeYSheet;
        animationState[state - 1] = true;
        verifyUniqueAnimation(state);

        switch (state)
        {
        case 1:
        {
            posXSheet = 0;
            characterSheet.generateImage(posXSheet, posYSheet, sizeXSheet, sizeYSheet, posX, postempY);
        }break;
        case 2:
        {
            if (frameTravel < frameCountWalk)
            {
                characterSheet.generateImage(posXSheet, posYSheet, sizeXSheet, sizeYSheet, posX, postempY);
                if (frameCounter == frameDurationWalk[frameTravel])
                {
                    frameCounter = 1;
                    frameTravel++;
                    posXSheet += sizeXSheet;
                }
                else
                    frameCounter++;
            }
            if (frameTravel == frameCountWalk)
            {
                posXSheet = 0;
                frameTravel = 0;
                frameCounter = 1;
            }
            avanzar();
        }break;
        case 3:
        {
            if (frameTravel < frameCountAttack)
            {
                characterSheet.generateImage(posXSheet, posYSheet, sizeXSheet, sizeYSheet, posX, postempY);
                if (frameCounter == frameDurationAttack[frameTravel])
                {
                    frameCounter = 1;
                    frameTravel++;
                    posXSheet += sizeXSheet;
                }
                else
                    frameCounter++;
            }
            if (frameTravel == frameCountAttack)
            {
                posXSheet = 0;
                frameTravel = 0;
                frameCounter = 1;
                animationState[2] = false;
            }
        }break;
        }
    }
    bool operator()(character* x, character* y)
    {
        if (x != nullptr && y != nullptr)
            return (x->getAddPosY() < y->getAddPosY());
        else if (y != nullptr)
            return true;
        else
            return false;
    }
}comparator;

void character::verifyUniqueAnimation(int state)
{
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        if (animationState[i])
        {
            count++;
            if (count >= 2)
            {
                animationState[0] = false; animationState[1] = false; animationState[2] = false;
                animationState[state - 1] = true;
                posXSheet = 0;
                frameTravel = 0;
                frameCounter = 1;
                return;
            }

        }
    }

}