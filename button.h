#pragma once
#include <iostream>
#include "data.cpp"
#include "image.h"
#include <Windows.h>
#include <WinUser.h>
using namespace std;

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

public:
	button(string name, int ax, int ay, void (*btn)(int&,int), int fL){
		setButtonFunction(name, ax, ay, btn, fL);
	}
	button(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL) :x(ax), y(ay), frameLimit(fL){
		setButtonFunction(name, ax, ay, btn, fL);
	}
	void setButtonAtributes(string name, int ax, int ay, int fL)
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
	}
	void setButtonFunction(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL)
	{
		setButtonAtributes(name, ax, ay, fL);
		buttonGenerate = btn;
		ButtonChangeInt = NULL;
	}
	void setButtonFunction(string name, int ax, int ay, void (*btn)(int&,int), int fL)
	{
		setButtonAtributes(name, ax, ay, fL);
		buttonGenerate = NULL;
		ButtonChangeInt = btn;
	}
	int getX() { return x; }
	int getY() { return y; }
	int getButtonWidth() { return buttonWidth; }
	int getButtonHeight() { return buttonHeight; }
	void rechargeButton(ALLEGRO_EVENT event)
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
						canTouch = true;
			}
			else
				gonnaPress = false;
		}
		else
		{
			gonnaPress = false;
		}

	}
	void setMode(bool mod) { mode = mod; }
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
				int n = getDB(ID, "price");
				fontMoney.modifyMoney(n, false, false);
				buttonGenerate(ID, team, quantity, isMine, fontMoney);
			}
		}
		else
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
						buttonGenerate(ID, team, quantity, isMine, fontMoney);
						frameCounter++;
					}
				}
			}
			else
				buttonIdle.generateImage(x, y);
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
	virtual void setValues(string, int, int, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int) = 0;
	virtual void setValues(string, int, int, void (*btnp)(int&,int), int) = 0;
	virtual void rechargeBtn(ALLEGRO_EVENT) = 0;
	virtual void execute(int, vector<character*>&, size_t&, bool, background&) = 0;
	virtual void execute(int&,int) = 0;
	virtual void setModes(bool aux) = 0;
};

class generateCommand : public commandI
{
private:
	button* btn;
public:
	generateCommand() : btn(nullptr){}
	generateCommand(button* _btn) : btn(_btn) {}
	generateCommand(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL)
	{
		btn = new button(name, ax, ay, btnp, fL);
	}
	~generateCommand() { delete btn; }
	void setValues(string name, int ax, int ay, void(*btnp)(int&,int), int fL) {}
	void setValues(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL) {
		btn->setButtonFunction(name, ax, ay, btnp, fL);
	}
	void rechargeBtn(ALLEGRO_EVENT event) {
		btn->rechargeButton(event);
	}
	void execute(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {
		btn->generateBtn(ID, team, quantity, isMine, fontMoney);
	}
	void execute(int&,int){}
	void setModes(bool aux)
	{
		btn->setMode(aux);
	}
};
class voidCommand : public commandI
{
private:
	button* btn;
public:
	voidCommand():btn(nullptr){}
	voidCommand(button* _btn) : btn(_btn) {}
	voidCommand(string name, int ax, int ay, void (*btnp)(int&,int), int fL)
	{
		btn = new button(name, ax, ay, btnp, fL);
	}
	~voidCommand() { delete btn; }
	void setValues(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL) {}
	void setValues(string name, int ax, int ay, void(*btnp)(int&,int), int fL) {
		btn->setButtonFunction(name, ax, ay, btnp, fL);
	}
	void rechargeBtn(ALLEGRO_EVENT event) {
		btn->rechargeButton(event);
	}
	void execute(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {}
	void execute(int&ventana, int cambio) {
		btn->generateBtn(ventana,cambio);
	}
	void setModes(bool){}
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
	void recharBtn(ALLEGRO_EVENT event) {
		mCmd->rechargeBtn(event);
	}

};