#pragma once
#include "Debug.h"
#include "Events.h"
#include "Player.h"
#include "BadEvent.h"
#include "GoodEvent.h"
#include <string>
#include <fstream>
#include <iostream>
class Draw
{
public:
	void DrawScene(Debug* Debugger,  Player* Player, int DrawState);
private:
	void DrawReport(Debug* Debugger,  Player* Player);
	void DrawBank(Debug* Debugger, int DrawLevel);
	void DrawWin();
	void DrawLose();
};

