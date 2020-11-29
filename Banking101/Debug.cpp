#include "Debug.h"

void Debug::Log(std::string message, ErrorSeverity error)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), error);//change the colour fo the text to  reflect the severity of the error
	std::cout << "##############################" << std::endl << std::endl;
	switch (error) //determines what shoud come before the the error message
	{
	case 2:
		std::cout << "Unimportant Error Detected, details: " << message << std::endl; 
		break;
	case 4:
		std::cout << "Serious Error Detected, details: " << message << std::endl;
		break;
	case 6:
		std::cout << "Minor Error Detected, details: " << message << std::endl;
		break;
	}
	std::cout << "\a";//alert sound
	std::cout << std::endl << "##############################" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//change the colour fo the text to normal
	system("PAUSE");
}
void Debug::Log(std::string message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Minor);//change the colour fo the text to yellow
	std::cout << "##############################" << std::endl << std::endl;
	std::cout <<  message << std::endl;
	std::cout << "\a";//alert sound
	std::cout << std::endl << "##############################" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//change the colour fo the text to normal
	system("PAUSE");
}
