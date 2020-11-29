#include "Investments.h"

bool Investments::AddInvestment(int Ammount, int Days)
{
	if (m_InvestmentList.size() < 10) 
	{
		m_InvestmentList[Ammount] = Days;
		return true;
	}
	else { std::cout << "You already Reached the Maximum number of investments" << std::endl; return false; }
}
void Investments::ShowInvestments(Debug* Debugger, Player* Player)
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
void Investments::UpdateInvestments(Debug* Debugger, Player* Player)
{
	std::map<int, int>::reverse_iterator CheckOnInvestment;
	for (CheckOnInvestment = m_InvestmentList.rbegin(); CheckOnInvestment != m_InvestmentList.rend(); ++CheckOnInvestment)
	{
		CheckOnInvestment->second = CheckOnInvestment->second - 1;
		if (CheckOnInvestment->second <1) 
		{
			std::cout << "###################################" << std::endl;
			std::cout << "        !Investments Completed!" << std::endl;
			std::cout << CheckOnInvestment->first <<"$ will be added to your vault." << std::endl;
			Player->SetMoney(CheckOnInvestment->first);
			Player->SetTrust(5);
			if (Player->ReturnTrust() > 100) { Player->SetTrust(-Player->ReturnTrust()); Player->SetTrust(100); }
			m_InvestmentList.erase(1);
			std::cout << "###################################" << std::endl;
			system("pause");
		}
	}
}


