#pragma once
#include <iostream>
#include <Windows.h>
const enum ErrorSeverity { Serious = 4, Minor = 6, Normal = 2 };
class Debug
{
public:
	void Log(std::string message, ErrorSeverity error); //I use thie one for actual errors, 4 is red, 6 is yellow and 2 is green
	void Log(std::string message); //I use this one for alerts to the player, mostly teling them than an input is invalid
};

