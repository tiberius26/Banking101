#pragma once
#include <iostream>
#include <string>
#include "Events.h"
#include "Player.h"
#include "Debug.h"
#include <map>

class Investments
{
private:
	std::map<int, int> m_InvestmentList;
public:
	bool AddInvestment(int Ammount, int Days);
	void ShowInvestments(Debug* Debugger, Player* Player);
	void UpdateInvestments(Debug* Debugger, Player* Player);
	//void LoadInvestments(Debug* Debugger, Player* Player);  //tried and failled to make this one
};

