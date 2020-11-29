#include "Shop.h"

void Shop::BuyManager(Player* Player, Debug* Debugger) //determines the order and what to do
{
	CheckBought(Player);
	DisplayShop();
	AskForItem(Debugger);
	if (m_BuyOption != 5) { ExecuteCommand(Player, Debugger); }
	std::cout << "Coming to the shop took 1 day"<<std::endl;
}
void Shop::CheckBought(Player* Player) //checks the shop's items based on whether or not the player owns them already
{
	if (Player->CheckItems("CCTV") == true)
	{
		m_ShopItems[0] = false;
	}
	if (Player->CheckItems("Furniture") == true)
	{
		m_ShopItems[1] = false;
	}
	if (Player->CheckItems("DonationBox") == true)
	{
		m_ShopItems[2] = false;
	}
	if (Player->CheckItems("Stocks") == true)
	{
		m_ShopItems[3] = false;
	}
}
void Shop::DisplayShop() //aka draw
{
	std::cout << "###################################" << std::endl;
	std::cout << "  Product:                 Price:" << std::endl;
	std::cout << "1.CCTV                     10000$" << std::endl;
	std::cout << "2.Furniture                15000$" << std::endl;
	std::cout << "3.Donation Box             20000$" << std::endl;
	std::cout << "4.Stocks                   50000$" << std::endl;
	std::cout << "5.Exit" << std::endl;
	std::cout << "###################################" << std::endl;
}
void Shop::AskForItem(Debug* Debugger)
{
	bool CheckInput = false;
	while (CheckInput == false) 
	{
		std::cout << "What do you want to do? (type the number of the action/product)" << std::endl; //prompts the user for a valid input
		std::cin >> m_BuyOption;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
		}
		else if (m_BuyOption < 1 || m_BuyOption > 6)
		{
			Debugger->Log("Invalud input, try again.");
		}
		else { CheckInput = true; }
	}
}
void Shop::ExecuteCommand(Player* Player, Debug* Debugger)// checks the input fo the player and if they ahve enough money for an item/if it exists
{
	if (m_BuyOption == 1 && Player->ReturnMoney() >= 10000 ) //I need to make sure the player can afford it first
	{
		if (m_ShopItems[0] == true) 
		{
			Player->AddItem("CCTV", 1);
			Player->SetMoney(-10000);
			std::cout << "Item Bought" << std::endl;
		}
		else { Debugger->Log("You already have this item, go back to the bank."); }
	}
	else if (m_BuyOption == 2 && Player->ReturnMoney() >= 15000)
	{
		if (m_ShopItems[1] == true)
		{
			Player->AddItem("Furniture", 1);
			Player->SetMoney(-15000);
			std::cout << "Item Bought" << std::endl;
		}
		else { Debugger->Log("You already have this item, go back to the bank."); }
	}
	else if (m_BuyOption == 3 && Player->ReturnMoney() >= 20000)
	{
		if (m_ShopItems[2] == true)
		{
		Player->AddItem("DonationBox", 1);
		Player->SetMoney(-20000);
		std::cout << "Item Bought" << std::endl;
		}
		else { Debugger->Log("You already have this item, go back to the bank."); }
	}
	else if (m_BuyOption == 4 && Player->ReturnMoney() >= 50000)
	{
		if (m_ShopItems[3] == true)
		{
			Player->AddItem("Stocks", 1);
			Player->SetMoney(-50000);
			std::cout << "Item Bought" << std::endl;
		}
		else { Debugger->Log("You already have this item, go back to the bank."); }
	}
	else 
	{
		Debugger->Log("You don't have enough money for that, go back to the bank."); //if the logic reached this it means that you can't afford the item you wanted
	}
}
