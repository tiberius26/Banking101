#include "InputObject.h"
InputObject::InputObject()
{
	m_Command = Help;
	m_HireNumber = 0;
	m_MoneyToBorrow = 0;
	m_MoneyToInvest = 0;
	m_CreditScore = 0; //made it a float initially because I experimented with division but I did not like the answers
	m_MaxInvestment = 0;
}
void InputObject::InputManager(Debug* Debugger,  Player* Player) //controls what methods are used
{
	Shop* Buymanager = new Shop;
	AskForInput(Debugger);
	bool CheckInput = false;
	while (CheckInput == false) 
	{
		if (EnactInput(Debugger, Player, Buymanager) == false) { system("CLS"); AskForInput(Debugger); }//infinite loop until a valid input is given to enact input
		else { CheckInput = true;}
	}
	delete Buymanager;
	Buymanager = nullptr;
}
void InputObject::AskForInput(Debug* Debugger)
{
	std::string CommandInput;
	bool CheckInput = false;
	while (CheckInput == false)
	{
		system("CLS");
		std::cout << "###################################" << std::endl; //prompts the user for an input and checks if it is an acceptable input.
		std::cout << "Please take a decision on what to do." << std::endl;
		std::cout << "Type Help for the list of commands." << std::endl;
		std::cin >> CommandInput;
		if (CommandInput == "Help" || CommandInput == "help") { m_Command = Help; CheckInput = true; }
		else if (CommandInput == "Invest" || CommandInput == "invest") { m_Command = Invest; CheckInput = true;}
		else if (CommandInput == "Upgrade" || CommandInput == "upgrade") { m_Command = Upgrade; CheckInput = true;}
		else if (CommandInput == "Hire" || CommandInput == "hire") { m_Command = Hire; CheckInput = true;}
		else if (CommandInput == "Borrow" || CommandInput == "borrow") { m_Command = Borrow; CheckInput = true;}
		else if (CommandInput == "Exit" || CommandInput == "exit") { m_Command = Exit; CheckInput = true; }
		else if (CommandInput == "Skip" || CommandInput == "skip") { m_Command = Skip; CheckInput = true; }
		else if (CommandInput == "Buy" || CommandInput == "buy") { m_Command = Buy; CheckInput = true; }
		else 
		{
			std::cout << "###################################" << std::endl;
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
			system("CLS");
		}
	}
	std::cout << "###################################" << std::endl;
}
bool InputObject::EnactInput(Debug* Debugger, Player* Player,  Shop* buymanager)
{
	m_CreditScore = Player->ReturnMoney() * Player->ReturnTrust() * Player->ReturnLevel() + (Player->ReturnDays() * 100); //made it a float initially because I experimented with division btu I did not like the snawers
	m_MaxInvestment = 10000 * (Player->ReturnEmployeeCount() + 2) + (10000 * Player->ReturnLevel()); //credit score and maximum investment are varuables I use to keep the apleyer from borrowing/investing too much

	switch (m_Command)
	{
	case 0: //help:
		std::cout << "Type 'Invest' in order to invest money." << std::endl;
		std::cout << "Type 'Upgrade' in order to upgrade the bank." << std::endl;
		std::cout << "Type 'Hire' in order to hire new employees." << std::endl;
		std::cout << "Type 'Borrow' in order to take a loan." << std::endl;
		std::cout << "Type 'Exit' in order to Save & Exit. This will take your investments away." << std::endl;
		std::cout << "Type 'Skip' in order to go to the next day without doing anything." << std::endl;
		std::cout << "Type 'Buy' in order to go to the shop." << std::endl;
		std::cout << "For detailed instructions read the README file in the data folder." << std::endl;
		std::cout << "###################################" << std::endl;
		system("pause");
		return false;
		break;
	case 1: //invest:
		std::cout << "How much do you want to invest?" << std::endl;
		std::cin >> m_MoneyToInvest;
		if (std::cin.fail()) //checks if the input is valid
		{
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
			return false;
		}
		else if (Player->ReturnMoney() > m_MoneyToInvest && m_MoneyToInvest < m_MaxInvestment) //checkis if you have the money to invest and if you aren't investing too much
		{
			std::cout << "Investment Made" << std::endl;
			Player->SetMoney(-m_MoneyToInvest);
			int InvestmentReturn = m_MoneyToInvest + (m_MoneyToInvest * (Player->ReturnTrust() / 100)) + 1000 * Player->ReturnLevel(); // you gain more form the investment than you put in
			if (m_MoneyToInvest < 1000) { InvestmentReturn= InvestmentReturn / 2; }; //added a check because the return at 100 trust level 4 for small investments was too much
			std::cout << "You will make "<< InvestmentReturn<<"$ in 7 days." << std::endl;
			Player->AddInvestment(InvestmentReturn, 7);
			std::cout << "###################################" << std::endl;
			system("pause");
			return true; 
			}
		else
		{
			std::cout << "You can't make the investment!" << std::endl; 
			std::cout << "###################################" << std::endl;
			system("pause");
			return false;
		}
		break;
	case 2://upgrade: (chacks if you can level up or not)
		if (Player->ReturnLevel() == 1 && Player->ReturnMoney() > 15000) 
		{
			std::cout << "Upgrade Will be carried out for 15,000$" << std::endl; 
			Player->SetBankLevel(2);  
			Player->SetMoney(-15000);
			Player->RecalculateRent();
			std::cout << "###################################" << std::endl;
			system("pause");
			return true;
		}
		else if (Player->ReturnLevel() == 2 && Player->ReturnMoney() > 75000) 
		{
			std::cout << "Upgrade Will be carried out for 75,000$" << std::endl;
			Player->SetBankLevel(3);
			Player->SetMoney(-75000);
			Player->RecalculateRent();
			std::cout << "###################################" << std::endl;
			system("pause");
			return true; 
		}
		else if (Player->ReturnLevel() == 3 && Player->ReturnMoney() > 250000) 
		{
			std::cout << "Upgrade Will be carried out for 250,000$" << std::endl;
			Player->SetBankLevel(4);
			Player->SetMoney(-250000);
			Player->RecalculateRent();
			std::cout << "###################################" << std::endl;
			system("pause");
			return true; 
		}
		else if (Player->ReturnLevel() == 4) 
		{ 
			std::cout << "You are already at max rank" << std::endl;
			std::cout << "###################################" << std::endl;
			system("pause");
			return false;
		}
		else 
		{
			std::cout << "You don't have enough Money!"<< std::endl;
			std::cout << "###################################" << std::endl;
			system("pause");
			return false;
		}
		break;
	case 3://hire:
		std::cout << "How many empleyees do you want to hire?" << std::endl;
		std::cin >> m_HireNumber;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
			return false;
		}
		else if (Player->ReturnEmployeeCount()+ m_HireNumber > 2 * Player->ReturnLevel()) //you can have up to 8 employees (2x4)
		{
			std::cout << "You can't hire that many at your current level." << std::endl;
			std::cout << "###################################" << std::endl;
			system("pause");
			return false;
		}
		else
		{
			std::cout << "You got new Employees! You need to pay them 1000$ at the end of each week." << std::endl;
			Player->SetEmplayeeCount(m_HireNumber);
			std::cout << "###################################" << std::endl;
			system("pause");
			return true;
		}
		break;
	case 4://borrow:

		std::cout << "How much do you want to borrow?" << std::endl;
		std::cin >> m_MoneyToBorrow;
		if (m_CreditScore < m_MoneyToBorrow)//checks if you are allowed to borrow that much
		{ 
			std::cout << "Your credit score is too low!" << std::endl;
			std::cout << "###################################" << std::endl;
			system("pause");
			return false; 
		}
		else
		{
			std::cout << "You borrowed " << m_MoneyToBorrow << "$, "<< m_MoneyToBorrow *1.45 <<"$ will be colected at the end of the week." << std::endl;
			m_MoneyToBorrow = m_MoneyToBorrow * 1.45; //you need to pay interest
			Player->SetDebt(Player->ReturnDebt()+ m_MoneyToBorrow);
			Player->SetMoney(m_MoneyToBorrow);
			std::cout << "###################################" << std::endl;
			system("pause");
			return true;
		}
		break;
	case 5://exit
		Save(Player);
		std::cout << "###################################" << std::endl;
		system("pause");
		return true;
		break;
	case 6://skip (aka do nothing)
		return true;
		break;
	case 7://buy:
		buymanager->BuyManager(Player, Debugger);
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}
void InputObject::Save(Player* Player)//saves the game
{
	Player->SetSave(Player->ReturnSaves() + 1);//determines the number of the save
	char SaveLocation[50];
	sprintf_s(SaveLocation, "data/Saves/Save%d.ini", Player->ReturnSaves());
	bool NoDublicateSave = false;
	while (NoDublicateSave == false)
	{
		std::fstream CheckFile(SaveLocation);
		if (!CheckFile)
		{
			NoDublicateSave = true;
		}
		else
		{
			Player->SetSave(Player->ReturnSaves() + 1);//makes sure that the save is unique
			sprintf_s(SaveLocation, "data/Saves/Save%d.ini", Player->ReturnSaves());
		}

	}
	std::ofstream SaveFile(SaveLocation);//saves player stats:
	SaveFile << "Name=" << Player->ReturnName() << std::endl;
	SaveFile << "Money=" << Player->ReturnMoney() << std::endl;
	SaveFile << "Trust=" << Player->ReturnTrust() << std::endl;
	SaveFile << "EmployeeCount=" << Player->ReturnEmployeeCount() << std::endl;
	SaveFile << "Debt=" << Player->ReturnDebt() << std::endl;
	SaveFile << "DaysInBusiness=" << Player->ReturnDays() << std::endl;
	SaveFile << "BankLevel=" << Player->ReturnLevel() << std::endl;
	SaveFile << "InTheRed=" << Player->ReturnInTheRed() << std::endl;
	SaveFile << "DefeatCounter=" << Player->ReturnDefeatCounter() << std::endl;
	SaveFile << "SaveIndex=" << Player->ReturnSaves();
	SaveFile.close();
	char InventorySaveLocation[50];
	sprintf_s(InventorySaveLocation, "data/Saves/InventorySave%d.ini", Player->ReturnSaves());
	std::ofstream InventorySaveFile(InventorySaveLocation);//saves player inventory:
	InventorySaveFile << "GoldBar=" << Player->CheckItemsCount("GoldBar") << std::endl;
	InventorySaveFile << "Diamond=" << Player->CheckItemsCount("Diamond") << std::endl;
	InventorySaveFile << "Suit=" << Player->CheckItemsCount("Suit") << std::endl;
	InventorySaveFile << "Letter=" << Player->CheckItemsCount("Letter") << std::endl;
	InventorySaveFile << "Laptop=" << Player->CheckItemsCount("Laptop") << std::endl;
	InventorySaveFile << "Award=" << Player->CheckItemsCount("Award") << std::endl;
	InventorySaveFile << "Badge=" << Player->CheckItemsCount("Badge") << std::endl;
	InventorySaveFile << "Cactus=" << Player->CheckItemsCount("Cactus") << std::endl;
	InventorySaveFile << "Whisky=" << Player->CheckItemsCount("Whisky") << std::endl;
	InventorySaveFile << "Watch=" << Player->CheckItemsCount("Watch") << std::endl;
	InventorySaveFile << "Desk=" << Player->CheckItemsCount("Desk") << std::endl;
	InventorySaveFile << "Chair=" << Player->CheckItemsCount("Chair") << std::endl;
	InventorySaveFile << "CCTV=" << Player->CheckItemsCount("CCTV") << std::endl;
	InventorySaveFile << "Furniture=" << Player->CheckItemsCount("Furniture") << std::endl;
	InventorySaveFile << "DonationBox=" << Player->CheckItemsCount("DonationBox") << std::endl;
	InventorySaveFile << "Stocks=" << Player->CheckItemsCount("Stocks");

}

