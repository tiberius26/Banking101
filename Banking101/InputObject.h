#pragma once
#include "Events.h"
#include "Player.h"
#include "Debug.h"
#include "Shop.h"
const enum Inputs { Help = 0, Invest = 1, Upgrade = 2, Hire = 3, Borrow = 4, Exit = 5, Skip = 6, Buy = 7};
class InputObject
{
private:
	Inputs m_Command; // keeps track of the choice made by the player using the enum above
	int m_HireNumber; // how many people you want to hire
	int m_MoneyToBorrow; // how much money you want to borrow
	int m_MoneyToInvest; // how much money you want to invest
	int m_CreditScore; //a value to determine how much you can borrow //made it a float initially because I experimented with division btu I did not like the answers
	int m_MaxInvestment; //a value that I use to cap how much an investment can be
	void Save(Player* Player); //saves the game
	void AskForInput(Debug* Debugger); //prompts the user for an input
	bool EnactInput(Debug* Debugger, Player* Player, Shop* Buymanager); //carryes out the input
public:
	void InputManager(Debug* Debugger, Player* Player); //controls the order of the methods
	Inputs ReturnInput() { return m_Command; };
	InputObject();
};

