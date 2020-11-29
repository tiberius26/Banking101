#pragma once
#include "Player.h"
#include "Debug.h"
#include <array>
class Shop
{
private:
	int m_BuyOption; //stores the value for the item the player wants to buy
	std::array<bool, 4> m_ShopItems = { true,true,true,true}; //an array used to keep track of what items are in the shop
	void DisplayShop();//preety much a draw function
	void CheckBought(Player* Player); //Checks if items from the shop are owned by the player
	void AskForItem(Debug* Debugger); //prompts the user for a valid input, made it separate because I wanted to expand the shop to do more but gave up on it in the end
	void ExecuteCommand(Player* Player, Debug* Debugger); //determines if the player can affort the item/if the item is in stock and acts based on the information
public:
	void BuyManager(Player*Player, Debug* Debugger);//controls the order of the other methods

};

