#include "BadEvent.h"
BadEvent::BadEvent() //set some random information for the event variables
{
	std::srand(std::time(nullptr)); //set a seed for the random number generation
	m_EventName = "";
	m_Description = "";
	m_EmployeeDecrease = 0;
	m_TrustDecrease = 0;
}
void BadEvent::EventManager(Player* Player, Debug* Debug)
{
	if (EventChance() == true) //checks if an event should happen and carries it out
	{
		ChooseEvent(Debug, Player);
		DisplayEvent(Debug, Player);
		EnactEvent(Player);
		ClearEvent();
	}
}
bool BadEvent::EventChance()
{
	int chance = std::rand() % 100 + 1;
	if (chance <= 10) // 10% chance of a bad event
	{
		return true;
	}
	return false;
}
void BadEvent::ChooseEvent(Debug* Debug, Player* Player)
{
	int equalizer = std::rand() % 100 + 1;//wanted to make event 12 the least likely to happen because it loses you 90% of your money
	if (equalizer > 5) { m_EventIndex = std::rand() % 11 + 1; } //any bad event except for:
	else { m_EventIndex = 12; } //credit crush
	std::fstream EventData;
	std::string EventDataLine;

	char FileToLoad[50];
	sprintf_s(FileToLoad, "data/BadEvents/BadEvent%d.ini", m_EventIndex); //loads the data of the event
	EventData.open(FileToLoad);
	if (!EventData.is_open()) {
		char Failure[50];
		sprintf_s(Failure, "Error reading bad event file %d", m_EventIndex); //something went wrong...
		Debug->Log(Failure, Serious);
	}
	while (!EventData.eof())
	{
		std::getline(EventData, EventDataLine);
		auto LineSplitter = EventDataLine.find("=");
		std::string Id = EventDataLine.substr(0, LineSplitter);//where to start and how much to go
		m_EventData[Id] = EventDataLine.substr(LineSplitter + 1, EventDataLine.size() - (LineSplitter + 1)); //stores the data from the read line
	}
	EventData.close();
}
void BadEvent::DisplayEvent(Debug* Debug, Player* Player)
{
	std::cout << std::endl << "###################################" << std::endl;
	std::cout << "!" << m_EventData["EventName"] << "!" << std::endl;
	std::cout << m_EventData["Description"] << std::endl;
	std::fstream EventArt;
	std::string EventArtLineLine;
	std::string ToDraw;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	char FileToLoad[50];
	sprintf_s(FileToLoad, "data/BadEventsArt/BadEventArt%d.ini", m_EventIndex);//drawing here because I want to have different error messages and art locations without having extra checks
	EventArt.open(FileToLoad);
	if (!EventArt.is_open()) {
		char Failure[50];
		sprintf_s(Failure, "Error reading bad event art %d", m_EventIndex);
		Debug->Log(Failure, Serious);
	}
	while (!EventArt.eof())
	{
		std::getline(EventArt, EventArtLineLine);
		ToDraw += EventArtLineLine + "\n";
	}
	EventArt.close();
	std::cout << ToDraw; //draw the art for the event
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void BadEvent::EnactEvent(Player* Player)
{
	m_MoneyLost = Player->ReturnMoney() * stof(m_EventData["MoneyLost"]); //calculates the amount of money you lose
	if (Player->ReturnMoney() < 0) { std::cout << "Lucky that we had nothing to lose."<< std::endl; } //if you have no money you can't lose money
	else {
	Player->SetMoney((int)-m_MoneyLost); //takes money away from you, cast it as an int just to be safe
	std::cout << "This event lost the bank " << m_MoneyLost << "$" << std::endl;
	}

	m_EmployeeDecrease = stoi(m_EventData["EmployeeDecrease"]); //calculates the number of Employees you lose
	if (m_EmployeeDecrease != 0 && Player->ReturnEmployeeCount() > 0) //checks if the event is meant to take your employees/ if you ahve any
	{ 
		Player->SetEmplayeeCount(-m_EmployeeDecrease);
		std::cout << "We also lost " << m_EmployeeDecrease << " employees" << std::endl;
	}
	else { std::cout << "This event has no effect on our employees" << std::endl;}
	if (Player->ReturnEmployeeCount() < 1) { Player->SetEmplayeeCount(Player->ReturnEmployeeCount() * -1); } //if employees is <0 add the negative to reset it to 0 (if yo are at -1 it will add 1 to go to 0)

	m_TrustDecrease = stoi(m_EventData["TrustDecrease"]); //decreases your trust:
	Player->SetTrust(-m_TrustDecrease);
	std::cout << "Trust in the bank fell by " << m_TrustDecrease <<"%"<< std::endl;
	if (Player->ReturnTrust() < 1) { Player->SetTrust(Player->ReturnTrust()*-1);} //if trust is <0 add the negative to reset it to 0

	std::cout << std::endl << "###################################" << std::endl;
	system("pause");
}
void BadEvent::ClearEvent() //kept it separate so I can see it better
{
	m_EventData.clear();
}

