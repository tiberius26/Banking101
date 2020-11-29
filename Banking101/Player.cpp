#include "Player.h"

void Player::UpdatePlayer(Debug* Debugger)//draws the status of the player and investments and carryes out some ongoing costs/cashflow
{
	ShowInvestments();
	CalculateCosts(Debugger);
	PassiveGain();
	Draw(Debugger);
}

Player::Player() //defauld data for a new player  //adjust if the game is too difficult
{
	m_Debt = 0;
	m_Money = 5000;
	m_Trust = 1;
	m_EmployeeCount = 0;
	m_DaysInBusiness = 1;
	m_BankLevel =1;
	m_DefeatCounter = 0;
	m_InTheRed = false;
	m_Rent = 0;
	m_Saves = 0;
}
void Player::Draw(Debug* Debugger)
{
	std::cout << "###################################" << std::endl << std::endl; //ststus:

	std::cout << "Today's report: " << std::endl;
	std::cout << "Monay: " << m_Money << "$     ";
	std::cout << "Trust: " << m_Trust << "%" << std::endl;
	std::cout << "Employees: " << m_EmployeeCount;
	std::cout << "     Debt: " << m_Debt << "$" << std::endl << std::endl;
	std::cout << "Days in Business: " << m_DaysInBusiness << std::endl;

	std::cout << std::endl << "###################################" << std::endl;
	std::fstream Level;
	std::string ScoreLine;
	std::string FullLevel;
	char FileToLoad[50];
	sprintf_s(FileToLoad, "data/BankLevels/LV%d.ini", m_BankLevel); //A visual representation of how the bank looks
	Level.open(FileToLoad);
	if (!Level.is_open()) {
		std::cout << '\a';
		Debugger->Log("Error Opening Bank image file, go to data/BankLevels", Serious);
	}
	while (!Level.eof())
	{
		std::getline(Level, ScoreLine);
		FullLevel += ScoreLine + "\n";//storing each line of the bank ascii art
	}
	Level.close();
	std::cout << FullLevel << std::endl;//actually drawing the bank
	std::cout << std::endl << "###################################" << std::endl;
	system("pause");

}
void Player::CalculateCosts(Debug* Debugger)
{
	RecalculateRent(); //just to make sure that the rent is properly calculated
	if (m_DaysInBusiness > 20) { m_Rent += (m_DaysInBusiness - 20) * 10; } // a gradual difficulty increase
	bool DayCheck = !(m_DaysInBusiness % 7); // checks if a week has passed in the game
	if (DayCheck == true)
	{
		std::cout << "######################### " << std::endl;
		std::cout << "Your debt of " << m_Debt << "$ is due." << std::endl;
		SetMoney(-m_Debt); //automatically repay debt
		if (m_Money < m_Debt) { Debugger->Log("We don't have enough money to pay our debt!"); } //announcement for the player
		SetDebt(0); //resets debt
		int PayrollAmount = m_EmployeeCount * 1000; //calculates how much you need to pay your employees
		std::cout << "Payroll of " << PayrollAmount << "$ is due." << std::endl;
		if (m_Money < PayrollAmount && PayrollAmount != 0) //checks if you can/have to pay your employees
		{
			Debugger->Log("We don't have enough money to pay everyone!");
			SetEmplayeeCount(-1); //if you can't pay them 1 leaves
			if (m_EmployeeCount < 1) { m_EmployeeCount = 0; } //if employees is <0 add the negative to reset it to 0
			else { std::cout << " An Employee left." << std::endl; }
		}
		else { SetMoney(-PayrollAmount); }
		std::cout << "Rent of " << m_Rent << "$ is due." << std::endl;
		if (m_Money < m_Rent)
		{
			Debugger->Log("We don't have enough money to pay the rent!");
		}
		SetMoney(-m_Rent);
		if (m_Money < 0 && m_InTheRed == false) //checks if you are in the red
		{
			Debugger->Log("!You are now in the red! You have a week to bounce back or you will go bankrupt!");
			ToggleInTheRed();
		}
	}
	std::cout << "######################### " << std::endl;
}
bool Player::CheckWinLose()
{
	if (m_BankLevel == 4 && m_Trust == 100) //win conditions:
	{
		if (m_Money > 50000 && m_Debt < 10000)
		{
			if (CheckItems("Letter") == true && CheckItems("Badge"))
			{
				system("CLS");
				std::cout << std::endl << "##########################################################################################################################" << std::endl;
				std::cout << std::endl << "Congratulations " << m_Name <<"!"<< std::endl;
				std::cout << std::endl << "You have met the requirements to join our order." << std::endl;
				std::cout << std::endl << "Given our extensive sponsorship of your business... I know that you will make the right choice." << std::endl;
				std::cout << std::endl << "Welcome to CES." << std::endl;
				std::cout << R"(                                                                                                                                                             
 ____________________
|     CES Member    |
|  Banking Division |
|                   |
|   xxx-xxxx-xxxx   |
|    World Bank     |
|___________________|
				)" << std::endl;
				std::cout << std::endl << "##########################################################################################################################" << std::endl;
				system("pause");
				std::string Saving;//string for input
				std::cout << "###################################" << std::endl;
				int Score = m_BankLevel * m_Money + m_DaysInBusiness * m_EmployeeCount - m_Debt * 5;
				std::cout << "You had a score of " << Score << std::endl << std::endl;
				std::cout << "Do you want to save it? Yes/No" << std::endl;
				std::cin >> Saving;
				if (Saving == "Yes" || Saving == "yes")  //chacks if the user wants to save their score
				{
					std::ofstream log("data/Scores.ini", std::ios_base::app | std::ios_base::out); //chooses what file to add the score to
					log << std::endl << Score << "=" << m_Name; //adds the score adn the name of the player
				}
				std::cout << "###################################" << std::endl;
				return true;
			}
		}
	}
	if (m_InTheRed == true)
	{
		m_DefeatCounter++;
		if (m_Money > -1) { m_InTheRed = false; m_DefeatCounter = 0; } //checks if you are no longer in the red
		if (m_DefeatCounter == 7) //checks if you have been in the red for 7 days (lose condition)
		{
			system("CLS");
			std::cout << std::endl << "##########################################################################################################################" << std::endl;
			std::cout << std::endl << "Unfortunatelly you went bankrupt..." << std::endl;
			std::cout << std::endl << "You cannot be one of us." << std::endl;
			std::cout << R"(     
      ______________________________________________________________________________________________________
     |                                                                                                      |
     |           _________       _            ________          ___________      __________     _________   |
      |         |   ______|     | |          /        \        |  _________|    | _________|   |   ___   \   |
      |         |  |            | |         |   ____   |       | |              | |            |  |   \  |   |
       |        |  |            | |         |  |    |  |       | |_________     | |________    |  |    | |    |
       |        |  |            | |         |  |    |  |       |_________  |    | _________|   |  |    | |    |
        |       |  |            | |         |  |    |  |                 | |    | |            |  |    | |     |
        |       |  |______      | |____     |  |____|  |       __________| |    | |________    |  |___/  |     |
         |      |_________|     |______|     \________/        |___________|    |__________|   |_________/      |
         |______________________________________________________________________________________________________|
)" << std::endl;
			std::cout << std::endl << "##########################################################################################################################" << std::endl;
			system("pause");
			std::string Saving;//string for input
			std::cout << "###################################" << std::endl;
			int Score = m_BankLevel * m_Money + m_DaysInBusiness * m_EmployeeCount - m_Debt * 5;
			std::cout << "You had a score of " << Score << std::endl << std::endl;
			if (Score > 0) {
				std::cout << "Do you want to save it? Yes/No" << std::endl;
				std::cin >> Saving;
				if (Saving == "Yes" || Saving == "yes")  //chacks if the user wants to save their score
				{
					std::ofstream log("data/Scores.ini", std::ios_base::app | std::ios_base::out); //choses what file to add the score to
					log << std::endl << Score << "=" << m_Name; //adds the score adn the name of the player
				}
			}
			std::cout << "###################################" << std::endl;
			return true;
		}
	}
	return false;
}

void Player::PassiveGain() //calculates what passive benefit you gain form items you buy/get from events
{
	std::cout << "###################################" << std::endl;
	if (CheckItems("Suit") == true)
	{
		std::cout << "Your suit increased the people's trust." << std::endl;
		m_Trust += 1;
	}
	else { std::cout << "Suit not owned." << std::endl; }
	if (CheckItems("Cactus") == true)
	{
		std::cout << "We got compliments for the cactus." << std::endl;
		m_Trust+=1;
	}
	else { std::cout << "Cactus not owned." << std::endl; }
	if (CheckItems("CCTV") == true)
	{
		std::cout << "CCTV footage was used to recover some money." << std::endl;
		m_Money += 250;
	}
	else { std::cout << "CCTV cameras not owned." << std::endl; }
	if (CheckItems("Furniture") == true)
	{
		std::cout << "The furniture attracted some new people in." << std::endl;
		m_Trust += 1;
	}
	else { std::cout << "Furniture not owned." << std::endl; }
	if (CheckItems("DonationBox") == true)
	{
		std::cout << "Some People have donated money to the bank!" << std::endl;
		m_Money += 500;
	}
	else { std::cout << "Donation box not owned." << std::endl; }
	if (CheckItems("Stocks") == true)
	{
		std::cout << "We got some money from our stocks." << std::endl;
		m_Money += 1000;
	}
	else { std::cout << "Stocks not owned." << std::endl; }
	std::cout << "###################################" << std::endl;
	system("pause");
	if (m_Trust > 100) { m_Trust = 100; }
}
bool Player::CheckItems(std::string index) //checks if the player has the maximim amount of any given item
{
	if (index == "Letter" && m_Inventory[index] == 1) { return true; }
	else if(index == "Suit" &&  m_Inventory[index] == 1) { return true; }
	else if (index == "Diamond" && m_Inventory[index] == 3) { return true; }
	else if (index == "GoldBar" && m_Inventory[index] == 5) { return true; }
	else if (index == "CCTV" && m_Inventory[index] == 1) { return true; }
	else if (index == "Furniture" && m_Inventory[index] == 1) { return true; }
	else if (index == "Stocks" && m_Inventory[index] == 1) { return true; }
	else if (index == "DonationBox" && m_Inventory[index] == 1) { return true; }
	else if (index == "Letter" && m_Inventory[index] == 1) { return true; }
	else if (index == "Laptop" && m_Inventory[index] == 1) { return true; }
	else if (index == "Award" && m_Inventory[index] == 1) { return true; }
	else if (index == "Badge" && m_Inventory[index] == 1) { return true; }
	else if (index == "Cactus" && m_Inventory[index] == 1) { return true; }
	else if (index == "Whisky" && m_Inventory[index] == 1) { return true; }
	else if (index == "Watch" && m_Inventory[index] == 1) { return true; }
	else if (index == "Desk" && m_Inventory[index] == 1) { return true; }
	else if (index == "Chair" && m_Inventory[index] == 1) { return true; }
	//std::cout << m_Inventory["Letter"];
	//std::cout << "//////////////////////////////////////////";
	return false;
}
void Player::LoadPlayer(std::string Name, int Money, int Trust, int Employees, int Debt, int Days, int Level, bool InTheRed, int DefeatCounter, int Saves) //stores the information given in the player (sent by the load)
{
	m_Name = Name;
	m_Money = Money;
	m_Trust = Trust;
	m_EmployeeCount = Employees;
	m_Debt = Debt;
	m_DaysInBusiness = Days;
	m_BankLevel = Level;
	m_InTheRed = InTheRed;
	m_DefeatCounter = DefeatCounter;
	m_Saves = Saves;
	RecalculateRent(); //because the level of the bank changed I do this for safety
}
void Player::LoadInventory(Debug* Debugger)
{
	std::fstream InventoryFile;
	std::string InventoryLine;
	char SaveLocation[50];
	sprintf_s(SaveLocation, "data/Saves/InventorySave%d.ini", m_Saves);//loads a save file
	InventoryFile.open(SaveLocation);
	if (!InventoryFile.is_open()) {
		Debugger->Log("Inventory save file not found", Serious); //if it doesn't exist it doesn't
	}
	while (!InventoryFile.eof())
	{
		std::getline(InventoryFile, InventoryLine);
		auto LineSplitter = InventoryLine.find("=");
		std::string Id = InventoryLine.substr(0, LineSplitter);//where to start and how much to go
		m_Inventory[Id] = stoi(InventoryLine.substr(LineSplitter + 1, InventoryLine.size() - (LineSplitter + 1)));
	}
}

void Player::UpdateInvestments()//updates the investments by 1 day and adds the money from any that have finished before deleting it
{
	std::map<int, int>::reverse_iterator CheckOnInvestment;
	for (CheckOnInvestment = m_InvestmentList.rbegin(); CheckOnInvestment != m_InvestmentList.rend(); ++CheckOnInvestment)
	{
		CheckOnInvestment->second = CheckOnInvestment->second - 1;
		if (CheckOnInvestment->second < 1)
		{
			std::cout << "###################################" << std::endl;
			std::cout << "        !Investments Completed!" << std::endl;
			std::cout << CheckOnInvestment->first << "$ will be added to your vault." << std::endl;
			SetMoney(CheckOnInvestment->first);
			SetTrust(5);
			if (m_Trust > 100) { m_Trust = 100; }
			m_InvestmentList.erase(1);
			std::cout << "###################################" << std::endl;
			system("pause");
		}
	}
}
void Player::ShowInvestments() //draw for investments
{
	std::cout << "###################################" << std::endl; //display the investments:
	std::cout << "        !Investments!" << std::endl;
	std::cout << "   Days           Amount" << std::endl << std::endl;

	std::map<int, int>::reverse_iterator CheckOnInvestment;
	int ScoreStanding = 1; //a number used to determine the rank someone has
	for (CheckOnInvestment = m_InvestmentList.rbegin(); CheckOnInvestment != m_InvestmentList.rend(); ++CheckOnInvestment)
	{
		std::cout << ScoreStanding << ". " << CheckOnInvestment->first << "     -     " << CheckOnInvestment->second << "\n"; //loop through all the players and display their high scores
		ScoreStanding++;
	}
	std::cout << std::endl << "###################################" << std::endl;
	system("pause");
}

bool Player::AddInvestment(int Ammount, int Days)
{
	if (m_InvestmentList.size() < 10)
	{
		m_InvestmentList[Ammount] = Days;
		return true;
	}
	else { std::cout << "You already Reached the Maximum number of investments" << std::endl; return false; }
}
