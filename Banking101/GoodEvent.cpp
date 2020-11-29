#include "GoodEvent.h"
GoodEvent::GoodEvent()//set some random information for the event variables
{
	std::srand(std::time(nullptr));//set a seed for the random number generation based on time
	m_EventName = "";
	m_Description = "";
	m_ItemName = "";
	m_ItemCount = 0;
}
void GoodEvent::EventManager(Player* Player, Debug* Debug)
{
	if (EventChance() == true)//checks if an event should happen and carries it out
	{
		ChooseEvent(Debug, Player);
		DisplayEvent(Debug, Player);
		EnactEvent(Player);
		ClearEvent();
	}
}
bool GoodEvent::EventChance()
{
	int chance = std::rand() % 100 + 1;
	if (chance <= 20)// 20% chance of a good event
	{
		return true;
	}
	return false;
}
void GoodEvent::ChooseEvent(Debug* Debug, Player* Player)
{
	m_EventIndex = std::rand() % 12 + 1;//determines what event to load
	std::fstream EventData;
	std::string EventDataLine;

	char FileToLoad[50];
	sprintf_s(FileToLoad, "data/GoodEvents/GoodEvent%d.ini", m_EventIndex);//loads the data of the event
	EventData.open(FileToLoad);
	if (!EventData.is_open()) {
		char Failure[50];
		sprintf_s(Failure, "Error reading good event file %d", m_EventIndex);//error checking...
		Debug->Log(Failure, Serious);
	}
	while (!EventData.eof())
	{
		std::getline(EventData, EventDataLine);
		auto LineSplitter = EventDataLine.find("=");
		std::string Id = EventDataLine.substr(0, LineSplitter);//where to start and how much to go
		m_EventData[Id] = EventDataLine.substr(LineSplitter + 1, EventDataLine.size() - (LineSplitter + 1));//stores the data from the read line
	}
	EventData.close();
}
void GoodEvent::DisplayEvent(Debug* Debug, Player* Player)
{
	std::cout << std::endl << "###################################" << std::endl;
	std::cout <<"!"<< m_EventData["EventName"]<<"!"<<std::endl;
	std::cout << m_EventData["Description"] << std::endl;
	if (Player->CheckItems(m_EventData["ItemName"]) == false)
	{
		Player->AddItem(m_EventData["ItemName"],1);
		std::fstream EventArt;
		std::string EventArtLineLine;
		std::string ToDraw;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		char FileToLoad[50];
		sprintf_s(FileToLoad, "data/GoodEventArt/GoodEventArt%d.ini", m_EventIndex); //drawing here because I want to have different error messages and art locations without having extra checks
		EventArt.open(FileToLoad);
		if (!EventArt.is_open()) {
			char Failure[50];
			sprintf_s(Failure, "Error reading good event art %d", m_EventIndex);
			Debug->Log(Failure, Serious);
		}
		while (!EventArt.eof())
		{
			std::getline(EventArt, EventArtLineLine);
			ToDraw += EventArtLineLine + "\n";
		}
		EventArt.close();
		std::cout << "The item you got:" << std::endl;
		std::cout << ToDraw;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else
	{
		std::cout << "Unfortunatelly we cannot hold any more of these, so we sold it for 5000$ (not added to total below)" << std::endl;
		Player->SetMoney(5000);
	}
}
void GoodEvent::EnactEvent(Player* Player)
{
	m_MoneyGained = stoi(m_EventData["MoneyGained"]);//aded it in a variable because I want a clear way of displaying it (line 92) instead of acessing it through the array
	Player->SetMoney(m_MoneyGained);
	std::cout << "This event brought the bank " << m_MoneyGained <<"$"<<std::endl;
	std::cout << std::endl << "###################################" << std::endl;
	system("pause");
}
void GoodEvent::ClearEvent() //kept it separate so I can see it better
{
	m_EventData.clear();
}
