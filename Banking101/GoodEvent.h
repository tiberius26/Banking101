#pragma once
#include "Events.h"
class GoodEvent : public Events
{
private:
	int m_ItemCount;
	int m_MoneyGained;
	std::string m_ItemName;
	bool EventChance() override;//this method is used to determine if a good event will happen
	void ChooseEvent(Debug* Debug, Player* Player) override;//this method determines which one of the 12 good events will happen
	void DisplayEvent(Debug* Debug, Player* Player) override;//shows the user relevant data about the event
	void EnactEvent(Player* Player) override;//carryes our event effects
	void ClearEvent() override;//clears the event data to avoid any problems
public:
	GoodEvent();
	void EventManager(Player* Player, Debug* Debug) override;;// determines the order the class is executed
};

