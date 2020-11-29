#pragma once
#include <iostream>
#include <string>
#include "Events.h"
#include "Player.h"
#include "Debug.h"
#include "Investments.h"
#include "InputObject.h"
class Update
{
private:
	int m_DefeatCounter;
	void CalculateDebt(Debug* Debugger, Player* Player);
	void Payroll(Debug* Debugger, Player* Player);
	void PassiveGains(Debug* Debugger, Player* Player);
public:
	void UpdateAll(Debug* Debugger, Events* Good, Events* Bad, Player* Player, Investments* Investment);
	bool CheckWinLoss(Debug* Debugger, Player* Player);
	Update() { m_DefeatCounter = 0; }
};

