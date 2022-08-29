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
protected: //Todos los atributos de character se encuentran aqui
    int ID;//--------------------------------
    int HP;
    int range;
    int damage;
    float attackSpeed;
    float speedMovement;
    bool areaDamage;
    int price; //Hasta aqui son caracteristicas

    float posX; //--------------------------------
    float posY;
    int addPosY; //Hasta aqui son posiciones
    bool isMine; //Determina si el personaje es tuyo o del enemigo

    int sizeXSheet;//--------------------------------
    int sizeYSheet;
    int posXSheet;
    int posYSheet;

    int hitboXOrigin;
    int hitboXEnd;

    int frameCountWalk;
    int frameCountAttack;
    int* frameDurationWalk;
    int* frameDurationAttack; 
    int frameAttack; //Todas las variables que están aquí son parámetros que usa la funcion de animar al personaje para hacer la ilusión de movimiento
    int focusID = -1; //ID del enemigo al que está apuntando el personaje

    bool animationState[3]{ false,false,false }; //Variables de animacion
    void verifyUniqueAnimation(int); //--------------------------------

    int frameTravel; //--------------------------------
    int frameCounter; //Variables de animacion

    image characterSheet; //SpriteSheet o imagen de imagenes del personaje
    sound soundAttack[2] = { sound("audio/attack/1.mp3"),sound("audio/attack/2.mp3") }; //Sonidos de ataque de los personajes

public:
    character() { reInitialize(0, true); }
    character(int auxID, bool propery) { reInitialize(auxID, propery); }
    int getID() { return ID; }
    int getPosX() { return posX; }
    int getHP() { return HP; }
    int getHitboxEnd() { return hitboXEnd; }
    int getAddPosY() { return addPosY; }
    void reInitialize(int auxID, bool propery) //Función general que sirve como constructor del personaje, con la ID es más que suficiente para rellenar todos los atributos
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
        HP = SingletonData::getDB(ID, "HP"); //Se usa la instancia de SingletonData para llamar a getDB y que extraiga todos los datos que necesitamos
        range = SingletonData::getDB(ID, "range");
        damage = SingletonData::getDB(ID, "damage");

        attackSpeed = SingletonData::getDB(ID, "attackSpeed", true);
        speedMovement = SingletonData::getDB(ID, "speedMovement", true);

        areaDamage = SingletonData::getDB(ID);
        price = SingletonData::getDB(ID, "price");

        sizeXSheet = SingletonData::getDB(ID, "sizeXSheet");
        sizeYSheet = SingletonData::getDB(ID, "sizeYSheet");
        frameCountWalk = SingletonData::getDB(ID, "frameCountWalk");
        frameDurationWalk = SingletonData::getDB(ID, 0);
        frameCountAttack = SingletonData::getDB(ID, "frameCountAttack");
        frameDurationAttack = SingletonData::getDB(ID, 1);
        hitboXOrigin = SingletonData::getDB(ID, "hitboXOrigin");
        hitboXEnd = SingletonData::getDB(ID, "hitboXEnd");
        frameTravel = 0;
        frameCounter = 1;
        frameAttack = SingletonData::getDB(ID, "frameAttack");

        posXSheet = 0;
        posYSheet = 0;
        posX = 50;
        if (!isMine) posX = 1870 - sizeXSheet;
        addPosY = (rand() % (8)) * 4;
        posY = 650 + addPosY;
    }
    virtual ~character() {}
    //Datos principales a guardar: ID HP frameTravel frameCounter posXSheet posYSheet posX posY 
    string returnDataActual() //estas funciones sirven para guardar el estado en el que estan los personajes al cerrar
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
    void setDataActual(vector<int>(*func)(string, string),string data) //seteamos todos los datos sacados de la base de datos (Funciones de main)
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
    void avanzar() { if (isMine) posX += 1 * speedMovement; else { posX -= 1 * speedMovement; } } //movimiento en x basado en speedMovement
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
    }/*Mediante espacios en coordenadas detecta si hay algun enemigo o una torre en frente suyo, para esto pasamos como
     parametro la torre o el vector de punteros a enemigos y analizamos por su ubicación si es posible atacar */

    virtual bool detect_enemie(vector<character*> enemies)
    {
        if (isMine)
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i] != NULL)
                    if (posX + hitboXEnd + range <= enemies[i]->getPosX() + 100 && posX + hitboXEnd + range >= enemies[i]->getPosX())
                    {
                        focusID = i; //En el caso del enemigo también guarda su ID en el vector de enemigos para poder atacarlo
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
                        focusID = i; //En el caso del enemigo también guarda su ID en el vector de enemigos para poder atacarlo
                        return true;
                    }
            }
        }
        return false;
    }
    virtual void atacar(vector<character*>& enemies, tower* enemieTower, bool isTower)
    {
        if (frameTravel == frameAttack - 1 && frameCounter == 1) //Verifica si los frames de animación ya estan en el frame indicado para atacar 
        {
            soundAttack[rand() % (2)].playSound(); //Lanza un sonido aleatorio
            if (isTower)
            {
                enemieTower->modifyHealth(damage, true); //Ataque a torre
                if (enemieTower->getHealth() <= 0)
                {
                    cout << "GANASTE" << endl; 
                }
            }
            else
            {
                enemies[focusID]->modifyHP(damage); //Ataque a enemigo
                if (enemies[focusID]->getHP() <= 0)
                {
                    delete enemies[focusID];
                    enemies[focusID] = NULL;
                    focusID = -1;
                }
            }
        }
    }
    virtual void generateCharacter(vector<character*>& enemies, tower* enemieTower) //Funcion que genera al personaje en pantalla, lo anima y hace que ataque y detecte
    {
        bool haveTower = detect_tower(enemieTower); //Detecta con prioridad si hay una torre
        bool haveEnemy = false;
        if (haveTower)
        {
            animateCharacter(3);
            atacar(enemies, enemieTower, true); //Animación y ataque
        }
        else if (!haveTower)
        {
            if (focusID >= 0)
                if (enemies[focusID] == NULL)
                    focusID = -1; //Reinicio del FocusID en caso de que ya no haya ningun enemigo
            haveEnemy = detect_enemie(enemies);
            if (haveEnemy)
            {
                animateCharacter(3); //Animacion y ataque
                atacar(enemies, enemieTower, false);
            }
            else if (animationState[2])
            {
                animateCharacter(3); //Hay prioridad de terminar la animación de ataque a pesar de que ya no hayan enemigos al frente
            }
            else
                animateCharacter(2); //Si no se cumple ninguna detección solo avanza
        }
    }
    void modifyHP(int x) { HP -= x; }

    void animateCharacter(int state) //linea de tiempo
    {
        int postempY = posY - sizeYSheet;
        posYSheet = (state - 1) * sizeYSheet;
        animationState[state - 1] = true;
        verifyUniqueAnimation(state); //Solo puede haber un estado de a

        switch (state)
        {
        case 1:
        {
            posXSheet = 0;
            characterSheet.generateImage(posXSheet, posYSheet, sizeXSheet, sizeYSheet, posX, postempY);
        }break;
        case 2:
        {
            if (frameTravel < frameCountWalk) //Se tiene la necesidad de viajar por la cantidad de frames que indique frameCountWalk
            {
                characterSheet.generateImage(posXSheet, posYSheet, sizeXSheet, sizeYSheet, posX, postempY); //Generación general
                if (frameCounter == frameDurationWalk[frameTravel])//cantidad de frames que tiene cada imagen que pasar
                {
                    frameCounter = 1;
                    frameTravel++;//Recorrido de los frames generales
                    posXSheet += sizeXSheet; //Recorrido del spriteSheet
                }
                else
                    frameCounter++;
            }
            if (frameTravel == frameCountWalk)
            {
                posXSheet = 0;
                frameTravel = 0;
                frameCounter = 1;//Reinicio
            }
            avanzar();
        }break;
        case 3:
        {
            if (frameTravel < frameCountAttack) //Sigue el mismo funcionamiento general del anterior caso
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
                animationState[2] = false; //Aseguramiento de que se cambie de estado de animacion al de caminar en caso no se cumplan los requerimientos
            }
        }break;
        }
    }
    bool operator()(character* x, character* y) //Functor de comparación de AddPosY, esto para ordenarlos sin que se superpongan
    {
        if (x != nullptr && y != nullptr)
            return (x->getAddPosY() < y->getAddPosY());
        else if (y != nullptr)
            return true;
        else
            return false;
    }
}comparator;

void character::verifyUniqueAnimation(int state) //Verificar que solo hay una animación activa
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