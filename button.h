#pragma once
#include <iostream>
#include "data.cpp"
#include "image.h"
#include <Windows.h>
#include <WinUser.h>
using namespace std;

constexpr unsigned int str2int(const char* str, int h = 0)
{return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];}
string upperCase(string word)
{
	string result;
	if (word.size() == 1)
	{
		char upperChar = word[0];
		if ((upperChar >= 'a') && (upperChar <= 'z'))
			upperChar += 'A' - 'a';
		result += upperChar;
	}
	else
	{
		for (int i{ 0 }; i < word.size(); i++)
		{
			char upperChar = word[i];
			if ((upperChar >= 'a') && (upperChar <= 'z'))
				upperChar += 'A' - 'a';
			
			result += upperChar;
		}
	}
	return result;
}
class button
{
protected:
	int x, y;
	int buttonWidth, buttonHeight;
	void (*ButtonChangeInt)(int&,int);
	void (*buttonGenerate)(int, vector<character*>&, size_t&, bool, background&);
	image buttonIdle;
	image buttonPress;
	bool canTouch;
	bool gonnaPress;
	bool activate;
	int frameCounter;
	int frameLimit;

	bool mode;
	int randomFrameMode = 0;
	int modeFramesCounts = 0;

	string keyControl;

public:
	button(string name, int ax, int ay, void (*btn)(int&,int), int fL, string keyControl){
		setButtonFunction(name, ax, ay, btn, fL, keyControl);
	}
	button(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL, string key){
		setButtonFunction(name, ax, ay, btn, fL,key);
	}
	void setButtonAtributes(string name, int ax, int ay, int fL, string key)
	{
		x = ax;
		y = ay;
		frameLimit = fL;
		string tempName = "buttons/" + name + "_iddle";
		buttonIdle.setBitmap(tempName, ".png");
		tempName = "buttons/" + name + "_press";
		buttonPress.setBitmap(tempName, ".png");
		buttonWidth = buttonIdle.getWidth();
		buttonHeight = buttonIdle.getHeight();
		canTouch = false;
		gonnaPress = false;
		frameCounter = 0;
		activate = true;
		mode = false;
		keyControl = upperCase(key);
	}
	void setButtonFunction(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL, string key)
	{
		setButtonAtributes(name, ax, ay, fL, key);
		buttonGenerate = btn;
		ButtonChangeInt = NULL;
	}
	void setButtonFunction(string name, int ax, int ay, void (*btn)(int&,int), int fL, string key)
	{
		setButtonAtributes(name, ax, ay, fL,key);
		buttonGenerate = NULL;
		ButtonChangeInt = btn;
	}
	int getX() { return x; }
	int getY() { return y; }
	int getButtonWidth() { return buttonWidth; }
	int getButtonHeight() { return buttonHeight; }
	void rechargeButton(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyState)
	{
		POINT mouse;
		GetCursorPos(&mouse);
		if (activate)
		{
			if (mouse.x > x && mouse.x < x + buttonWidth && mouse.y>y && mouse.y < y + buttonHeight)
			{
				gonnaPress = true;
				if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
					if (event.mouse.button & 1)
						canTouch = true; return;
			}
			else
				gonnaPress = false;
			const char* name = keyControl.c_str();
			switch (str2int(name))
			{
				case str2int("A"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_A))
						canTouch = true; return;
				}break;
				case str2int("S"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_S))
						canTouch = true; return;
				}break;
				case str2int("D"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_D))
						canTouch = true; return;
				}break;
				case str2int("J"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_J))
						canTouch = true; return;
				}break;
				case str2int("K"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_K))
						canTouch = true; return;
				}break;
				case str2int("L"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_L))
						canTouch = true; return;
				}break;
				case str2int("1"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_1))
						canTouch = true; return;
				}break;
				case str2int("2"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_2))
						canTouch = true; return;
				}break;
				case str2int("BACKSPACE"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_BACKSPACE))
						canTouch = true; return;
				}break;
				case str2int("ESCAPE"):
				{
					if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
						canTouch = true; return;
				}break;
				default:
				{
					canTouch = false; return;
				}break;
			}
			
		}
		else
		{
			gonnaPress = false;
		}

	}
	void setMode(bool mod) { mode = mod; }
	string getData()
	{
		string result;
		result += to_string(canTouch) + " ";
		result += to_string(gonnaPress) + " ";
		result += to_string(activate) + " ";
		result += to_string(frameCounter);
		return result;
	}
	void setData(vector<int>(*func)(string, string),string data)
	{
		vector<int> result = func(data, " ");
		canTouch = result[0];
		gonnaPress = result[1];
		activate = result[2];
		frameCounter = result[3];
	}
	void generateBtn(int& ventana, int cambio)
	{
		if (activate)
		{
			if (gonnaPress)
				buttonPress.generateImage(x, y);
			else
				buttonIdle.generateImage(x, y);
			if (frameCounter == 0)
			{
				if (canTouch)
				{
					canTouch = false;
					activate = false;
					ButtonChangeInt(ventana,cambio);
					frameCounter++;
				}
			}
		}
		else
			buttonIdle.generateImage(x, y);
		if (frameCounter > 0)
		{
			canTouch = false;
			frameCounter++;
			if (frameCounter == frameLimit)
			{
				frameCounter = 0;
				activate = true;
			}
		}
	}
	void generateBtn(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney)
	{
		if (mode)
		{
			if (modeFramesCounts == 0) {
				randomFrameMode = (4 + rand() % 3) * 60;
			}
			modeFramesCounts++;
			if (modeFramesCounts + 1 == randomFrameMode)
			{
				modeFramesCounts = 0;
				int n = SingletonData::getDB(ID, "price");
				fontMoney.modifyMoney(n, false, false);
				buttonGenerate(ID, team, quantity, isMine, fontMoney);
			}
		}
		else
		{
			if (fontMoney.getMoney(isMine) < SingletonData::getDB(ID, "price") || frameCounter != 0)
			{
				activate = false;
			}else{ activate = true;}
			if (activate)
			{
				if (gonnaPress)
					buttonPress.generateImage(x, y);
				else
					buttonIdle.generateImage(x, y);
				if (frameCounter == 0)
				{
					if (canTouch)
					{
						canTouch = false;
						activate = false;
						buttonGenerate(ID, team, quantity, isMine, fontMoney);
						frameCounter++;
					}
				}
			}
			else
				buttonPress.generateImage(x, y);
		}
		if (frameCounter > 0)
		{
			canTouch = false;
			frameCounter++;
			if (frameCounter == frameLimit)
			{
				frameCounter = 0;
				activate = true;
			}
		}
	}
};

class commandI
{
public:
	virtual void setValues(string, int, int, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int,string) = 0;
	virtual void setValues(string, int, int, void (*btnp)(int&,int), int, string) = 0;
	virtual void rechargeBtn(ALLEGRO_EVENT, ALLEGRO_KEYBOARD_STATE) = 0;
	virtual void execute(int, vector<character*>&, size_t&, bool, background&) = 0;
	virtual void execute(int&,int) = 0;
	virtual void setModes(bool aux) = 0;
	virtual string saveData() = 0;
	virtual void setData(vector<int>(*func)(string, string),string) = 0;
};

class generateCommand : public commandI
{
private:
	button* btn;
public:
	generateCommand() : btn(nullptr){}
	generateCommand(button* _btn) : btn(_btn) {}
	generateCommand(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL, string key)
	{btn = new button(name, ax, ay, btnp, fL, key);}
	~generateCommand() { delete btn; }
	void setValues(string name, int ax, int ay, void(*btnp)(int&,int), int fL, string key) {}
	void setValues(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL, string key) {
		btn->setButtonFunction(name, ax, ay, btnp, fL,key);
	}
	void rechargeBtn(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyState) {
		btn->rechargeButton(event,keyState);
	}
	void execute(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {
		btn->generateBtn(ID, team, quantity, isMine, fontMoney);
	}
	void execute(int&,int){}
	void setModes(bool aux)
	{
		btn->setMode(aux);
	}
	string saveData() { return btn->getData(); }
	void setData(vector<int>(*func)(string, string),string data) { btn->setData(func,data); }
};
class voidCommand : public commandI
{
private:
	button* btn;
public:
	voidCommand():btn(nullptr){}
	voidCommand(button* _btn) : btn(_btn) {}
	voidCommand(string name, int ax, int ay, void (*btnp)(int&,int), int fL, string key)
	{btn = new button(name, ax, ay, btnp, fL,key);}
	~voidCommand() { delete btn; }
	void setValues(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL, string key) {}
	void setValues(string name, int ax, int ay, void(*btnp)(int&,int), int fL, string key) {
		btn->setButtonFunction(name, ax, ay, btnp, fL, key);
	}
	void rechargeBtn(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyState) {
		btn->rechargeButton(event, keyState);
	}
	void execute(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {}
	void execute(int&ventana, int cambio) {
		btn->generateBtn(ventana,cambio);
	}
	void setModes(bool){}
	string saveData() { return btn->getData(); }
	void setData(vector<int>(*func)(string, string),string data) { btn->setData(func,data); }
};
class RemoteControl
{
private:
	commandI* mCmd;
public:
	RemoteControl() :mCmd(nullptr) {}
	RemoteControl(commandI* cmd) { mCmd = cmd; }
	~RemoteControl() { delete mCmd; }
	void setCommand(commandI* cmd) { mCmd = cmd; }
	void executeButton(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {
		mCmd->execute(ID, team, quantity, isMine, fontMoney);
	}
	void executeButton(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney, bool mode) {
		mCmd->execute(ID, team, quantity, isMine, fontMoney);
		mCmd->setModes(mode);
	}
	void executeButton(int& ventana, int cambio)
	{
		mCmd->execute(ventana, cambio);
	}
	void recharBtn(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyState) {
		mCmd->rechargeBtn(event, keyState);
	}
	string saveData() { return mCmd->saveData(); }
	void setData(vector<int>(*func)(string, string),string data) { mCmd->setData(func,data); }
};