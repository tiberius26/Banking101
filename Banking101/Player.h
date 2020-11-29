#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include "Debug.h"
class Player //bank manager
{
private:
	std::string m_Name;
	int m_Money;
	int m_Trust; //can people trust your bank
	int m_EmployeeCount;
	std::map<std::string, int>m_Inventory; //the items you have colected, counted
	std::map<int, int> m_InvestmentList; //used to keep track of investments
	int m_Debt; //stop taking money from loan sharks
	int m_DaysInBusiness; //I use this one for cost calculations and a gradual difficulty increase
	int m_BankLevel;
	bool m_InTheRed; //used to determine if the player has less than 0$
	int m_Saves; //used to make sure that the game saves in a new file
	int m_DefeatCounter; //I use this one to keep track of when the player loses
	int m_Rent;
	void Draw(Debug* Debugger);
	void CalculateCosts(Debug* Debugger); //pays dept, employees and rent
	void PassiveGain();
public:

	void UpdatePlayer(Debug* Debugger);//named it update because it updates the menus with new information
	Player();
	//methods that deal with addition
	void SetMoney(int MoneyChange) { m_Money += MoneyChange; };
	void SetTrust(int TrustChange) { m_Trust += TrustChange; };
	void SetEmplayeeCount(int EmplayeeNumChange) { m_EmployeeCount += EmplayeeNumChange; };
	void AddItem(std::string ItemName, int quantity) { m_Inventory[ItemName] += quantity;};
	bool AddInvestment(int Ammount, int Days);
	//methods that set variables
	void SetDebt(int DebtChange) { m_Debt = DebtChange; };
	void SetName(std::string name) { m_Name = name; };
	void SetBankLevel(int NewLevel) { m_BankLevel = NewLevel; };
	void SetSave(int SaveNumber) { m_Saves = SaveNumber; }
	//methods that increment variables
	void IncrementDays() { m_DaysInBusiness++; };
	//toggle methods
	void ToggleInTheRed() { if (m_InTheRed == true) { m_InTheRed = false; } else { m_InTheRed = true; } };
	void RecalculateRent() { m_Rent = 1500 * m_BankLevel; } // I think this is fair, might need to make it harder if too easy
	//return methods
	std::string ReturnName() { return m_Name; };
	int ReturnMoney() { return m_Money; };
	int ReturnTrust() { return m_Trust; };
	int ReturnEmployeeCount() { return m_EmployeeCount; };
	int ReturnDebt() { return m_Debt; };
	int ReturnDays() { return m_DaysInBusiness; };
	int ReturnLevel() { return m_BankLevel; };
	int ReturnSaves() { return m_Saves; };
	bool ReturnInTheRed() { return m_InTheRed; };
	int ReturnDefeatCounter() { return m_DefeatCounter; };
	//check methods
	bool CheckItems(std::string index); //checks if the player has the maximum amount of any given number
	int CheckItemsCount(std::string index){return m_Inventory[index];} //returns an item from the inventory based on the index given
	bool CheckWinLose(); //checks if the player has won/lost
	//loading methods
	void LoadPlayer(std::string Name, int Money, int Trust, int Employees, int Debt, int Days, int Level, bool InTheRed, int DefeatCounter, int Saves); //loads player stats
	void LoadInventory(Debug*Debugger); //loads player inventory
	void UpdateInvestments(); //updates the date for the investments
	void ShowInvestments(); //shows existing investments 


};

