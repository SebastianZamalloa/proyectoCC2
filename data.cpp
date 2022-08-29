#pragma once
#include <iostream>
using namespace std;
struct dataBase
{
    int ID;
    int type;

    int HP;
    int range;
    int damage;
    float attackSpeed;
    float speedMovement;
    bool areaDamage;
    int price;

    int sizeXSheet;
    int sizeYSheet;
    int hitboXOrigin;
    int hitboXEnd;

    int frameCountWalk;
    int frameCountAttack;
    int* frameDurationWalk;
    int* frameDurationAttack;
    int frameAttack;

    int extraData1;
    int extraData2;

}characterArray[3];
void dataIntialize()
{
    characterArray[0] = { 0,0, 0,0,0,0,0,false,0,0,0,0,0,0,0,NULL,NULL,0,0,0},
    characterArray[1] = { 1,1, 100,100,20,1.5,20,false,100,256,256,5,235,4,5,new int[4]{8,8,8,8},new int[5]{2,20,2,2,100},3,1,0 };
    characterArray[2] = { 2,1, 60,100,40,1.5,2,true,180,256,256,5,235,4,5,new int[4]{8,8,8,8},new int[5]{1,10,1,1,100},3,3,100 };
}
int getDB(int ID, string name)
{
        if(name=="HP") { return characterArray[ID].HP; }
        if (name == "type") { return characterArray[ID].type; }
        if(name=="range"){return characterArray[ID].range; }
        if(name=="damage") { return characterArray[ID].damage; }
        if(name=="range") { return characterArray[ID].range; }
        if(name=="price") { return characterArray[ID].price; }
        if(name=="sizeXSheet") { return characterArray[ID].sizeXSheet; }
        if(name=="sizeYSheet") { return characterArray[ID].sizeYSheet; }
        if(name=="hitboXOrigin") { return characterArray[ID].hitboXOrigin; }
        if(name=="hitboXEnd") { return characterArray[ID].hitboXEnd; }
        if(name=="frameCountWalk") { return characterArray[ID].frameCountWalk; }
        if(name=="frameCountAttack") { return characterArray[ID].frameCountAttack; }        
        if(name=="frameAttack") { return characterArray[ID].frameAttack; }   
        if (name == "extraData1") { return characterArray[ID].extraData1; }
        if (name == "extraData2") { return characterArray[ID].extraData2; }
}
bool getDB(int ID){ return characterArray[ID].areaDamage;}
float getDB(int ID, string name, bool temp)
{
    if(temp)
    {
        if (name == "attackSpeed") { return characterArray[ID].attackSpeed; }
        if (name == "speedMovement") { return characterArray[ID].speedMovement; }
    }
}
int* getDB(int ID, int walkAttack)
{
    if (walkAttack == 0) { return characterArray[ID].frameDurationWalk; }
    if (walkAttack == 1) { return characterArray[ID].frameDurationAttack; }
}
void deleteData()
{
    for (int i = 1; i <= 1; i++)
    {
        delete[] characterArray[i].frameDurationWalk;
        delete[] characterArray[i].frameDurationAttack;
    }
}

