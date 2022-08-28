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
	void (*buttonVoid)(void);
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
	int modeFramesCounts;
public:
	button(string name, int ax, int ay, void (*btn)(void), int fL) :x(ax), y(ay), frameLimit(fL)
	{
		string tempName = "buttons/" + name + "_iddle";
		buttonIdle.setBitmap(tempName, ".png");
		tempName = "buttons/" + name + "_press";
		buttonPress.setBitmap(tempName, ".png");
		buttonWidth = buttonIdle.getWidth();
		buttonHeight = buttonIdle.getHeight();
		buttonGenerate = NULL;
		buttonVoid = btn;
		canTouch = false;
		gonnaPress = false;
		activate = true;
		frameCounter = 0;
		mode = false;
	}
	button(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL) :x(ax), y(ay), frameLimit(fL)
	{
		setButton(name, ax, ay, btn, fL);
	}
	void setButton(string name, int ax, int ay, void (*btn)(int, vector<character*>&, size_t&, bool, background&), int fL)
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
		buttonGenerate = btn;
		canTouch = false;
		gonnaPress = false;
		frameCounter = 0;
		activate = true;
		buttonVoid = NULL;
		mode = false;
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
	void generateBtn(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney)
	{
		if (mode)
		{
			if (modeFramesCounts == 0) {
				randomFrameMode = (4 + rand() % 3) * 60;
				cout << randomFrameMode << endl;

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
	virtual void rechargeBtn(ALLEGRO_EVENT) = 0;
	virtual void execute(int, vector<character*>&, size_t&, bool, background&) = 0;
	virtual void setModes(bool aux) = 0;
};

class generateCommand : public commandI
{
private:
	button* btn;
public:
	generateCommand(button* _btn) : btn(_btn) {}
	generateCommand(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL)
	{
		btn = new button(name, ax, ay, btnp, fL);
	}
	~generateCommand() { delete btn; }
	void setValues(string name, int ax, int ay, void (*btnp)(int, vector<character*>&, size_t&, bool, background&), int fL) {
		btn->setButton(name, ax, ay, btnp, fL);
	}
	void rechargeBtn(ALLEGRO_EVENT event) {
		btn->rechargeButton(event);
	}
	void execute(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney) {
		btn->generateBtn(ID, team, quantity, isMine, fontMoney);
	}
	void setModes(bool aux)
	{
		btn->setMode(aux);
	}
};
class RemoteControl
{
private:
	commandI* mCmd;
public:
	RemoteControl() :mCmd(NULL) {}
	RemoteControl(commandI* cmd) { mCmd = cmd; }
	~RemoteControl() {}
	void setCommand(commandI* cmd) { mCmd = cmd; }
	void executeButton(int ID, vector<character*>& team, size_t& quantity, bool isMine, background& fontMoney, bool mode) {
		mCmd->execute(ID, team, quantity, isMine, fontMoney);
		mCmd->setModes(mode);
	}
	void recharBtn(ALLEGRO_EVENT event) {
		mCmd->rechargeBtn(event);
	}

};