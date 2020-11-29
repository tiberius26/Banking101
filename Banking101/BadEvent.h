#pragma once
#include "Events.h"
const enum EventSeverity { Big = 4, Small = 6, Unimportant = 2 };
class BadEvent : public Events
{
private:
	int m_MoneyLost;
	int m_EmployeeDecrease;
	int m_TrustDecrease;
	bool EventChance() override; //this method is used to determine if a bad event will happen
	void ChooseEvent(Debug* Debug, Player* Player) override; //this method determines which one of the 12 bad events will happen, number 12 having the lowest chance
	void DisplayEvent(Debug* Debug, Player* Player) override; //shows the user relevant data about the event
	void EnactEvent(Player* Player) override; //carryes our event effects
	void ClearEvent() override;//clears the event data to avoid any problems
public:
	BadEvent();
	void EventManager(Player* Player, Debug* Debug) override;// determines the order the class is executed
};

