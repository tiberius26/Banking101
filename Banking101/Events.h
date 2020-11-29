#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <string>
#include <ctime>
#include "Events.h"
#include "Debug.h"
#include "Player.h"
#include <array>
class Events
{
protected:
	std::string m_EventName;
	std::string m_Description;
	int m_EventIndex; //this variable is used to determine what event data and art to load
	std::map<std::string, std::string> m_EventData; //this is where the information for the event is stored
public:
    // = 0 because I only want good/bad events
	virtual void DisplayEvent(Debug* Debug, Player* Player) = 0;
	virtual void ChooseEvent(Debug* Debug, Player* Player) = 0;
	virtual void EnactEvent(Player* Player) = 0;
	virtual void EventManager(Player* Player, Debug* Debug) = 0;
	virtual bool EventChance() = 0;
	virtual void ClearEvent() = 0;
};

