#include <iostream>
#include <Windows.h>
#include "Debug.h"
#include <map>
#include <fstream>
#include <string>
#include "Player.h"
#include "Events.h"
#include "BadEvent.h"
#include "GoodEvent.h"
#include "Update.h"
#include "InputObject.h"
//game loop methods:
void GameLoop(Debug* Debugger, Player* Player);//contains the main loop for this game
//Menu methods:
void MainMenu(Debug* Debugger);//the first menu of the game
void HighScore(Debug* Debugger);//reads and shows the high scores
void Initialize(Debug* Debugger);//creates a NEW player and starts the game
void LoadSave(Debug* Debugger);//loads the data of a previous player and starts the game
void ExitMethod(Debug* Debugger); //displays a message before the application closes
void MenuSwitch(Debug* Debugger, int Choice);// a switch for the optionf on the main menu

int main()
{
	Debug* Debugger = new Debug;
	//change console window size to
	HWND Window = GetConsoleWindow(); //gets me the console
	RECT Size; //rectangle has a top, botton, right and left
	GetWindowRect(Window, &Size); //returns a bool with true if it can get them
	if (!MoveWindow(Window, Size.left, Size.top, 8000, 1000, TRUE))//changes window size if sucessful
	{
		Debugger->Log("Failled to Resize Window, Please do it manually", Minor); //Logs the error
	}
	MainMenu(Debugger);
	return 0;
}
void MainMenu(Debug* Debugger)
{
	int MenuChoice;
	bool CheckMenuInput = false;
	while (CheckMenuInput == false)//infinite loop until you give a valid input
	{
		system("CLS");
		std::cout << "###################################" << std::endl << std::endl;
		std::cout << "         Banking 101" << std::endl << std::endl;
		std::cout << "         !Main Menu!" << std::endl << std::endl;
		std::cout << "          1.   Play" << std::endl;
		std::cout << "          2.   Load" << std::endl;
		std::cout << "          3. HighScore" << std::endl;
		std::cout << "          4.   Exit" << std::endl << std::endl;
		std::cout << std::endl << "###################################" << std::endl;
		std::cin >> MenuChoice;
		if (std::cin.fail()) //checks if you give it somethign other than numbers
		{
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
		}
		else if (MenuChoice > 4 || MenuChoice < 1)//checks if your input is out of range
		{
			Debugger->Log("Invalud input, try again.");
			CheckMenuInput = false;
		}
		else 
		{ 
			CheckMenuInput = true; 
		}
	}
	system("CLS");
	MenuSwitch(Debugger, MenuChoice);


}
void MenuSwitch(Debug* Debugger, int Choice) //the switch that performs the command from the menu
{
	switch (Choice)
	{
	default:
		break;
	case 1:
		Initialize(Debugger);
		break;
	case 2:
		LoadSave(Debugger);
		break;
	case 3:
		HighScore(Debugger);
		break;
	case 4:
		ExitMethod(Debugger);
		break;
	}
}
void Initialize(Debug* Debugger)  //creates a new player
{
	std::cin.clear();
	std::cin.ignore(999, '\n');
	std::string TempName;
	Player* player = new Player;
	std::cout << "#########################################"<<std::endl;
	std::cout << "Before we start, Please enter your name: ";
	std::cin >> TempName;
	player->SetName(TempName);
	GameLoop(Debugger, player);
}
//Trying to separate gameloop from these two is a nightmare, because it needs the player to be sent to it, I considered a few solutions but they required too many changes I didn't want to make
void LoadSave(Debug* Debugger) //loads a save file
{
	Player* player = new Player;
	std::fstream Options;
	std::string OptionLine;
	std::map<std::string, std::string> SettupOptions;
	int SaveToLoad;
	bool SaveCheck = false;
	while (SaveCheck == false)
	{
		system("CLS");
		std::cout << "Please enter the number of the save file you wish to load. If there are no saves, please restart the application." << std::endl;
		SaveCheck = true; //I set it true immediatelly but this is changed to false if the input is invalid below.
		std::cin >> SaveToLoad;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(999, '\n');
			Debugger->Log("Invalud input, try again.");
			SaveCheck = false;
		}
		char SaveLocation[50];
		sprintf_s(SaveLocation, "data/Saves/Save%d.ini", SaveToLoad);
		Options.open(SaveLocation);
		if (!Options.is_open()) {
			Debugger->Log("File not found, try again", Serious);
			SaveCheck = false;
		}
	}
	while (!Options.eof())//loads te stats and stores them in a map
	{
		std::getline(Options, OptionLine);
		auto LineSplitter = OptionLine.find("=");
		std::string Id = OptionLine.substr(0, LineSplitter);//where to start and how much to go
		SettupOptions[Id] = OptionLine.substr(LineSplitter + 1, OptionLine.size() - (LineSplitter + 1));
	}
	Options.close();
	bool DefeatCheck;
	if (SettupOptions["InTheRed"] == "true") { DefeatCheck = true; }
	else { DefeatCheck = false; }
	player->LoadPlayer(SettupOptions["Name"], stoi(SettupOptions["Money"]), stoi(SettupOptions["Trust"]), stoi(SettupOptions["EmployeeCount"]), stoi(SettupOptions["Debt"]), stoi(SettupOptions["DaysInBusiness"]), stoi(SettupOptions["BankLevel"]), DefeatCheck, stoi(SettupOptions["DefeatCounter"]), stoi(SettupOptions["SaveIndex"])); //sends all the read information to the player
	player->LoadInventory(Debugger);//loads the inventory, I do in inplayer because I don't want this method to get too big
	GameLoop(Debugger, player);

}
void HighScore(Debug* Debugger)
{
	system("CLS");
	std::fstream Scores;
	std::string ScoreLine;
	std::map<int, std::string> HighScores;
	Scores.open("data/Scores.ini");
	if (!Scores.is_open()) {
		std::cout << '\a';
		Debugger->Log("Error reading score file.", Serious);
		MainMenu(Debugger);
	}
	while (!Scores.eof())
	{
		std::getline(Scores, ScoreLine);
		auto LineSplitter = ScoreLine.find("=");
		std::string ScoreAsString = ScoreLine.substr(0, LineSplitter);//where to start and how much to go
		int Id = std::stoi(ScoreAsString);
		HighScores[Id] = ScoreLine.substr(LineSplitter + 1, ScoreLine.size() - (LineSplitter + 1));//stores each name with their highscore

	}
	Scores.close();

	std::cout << "###################################" << std::endl << std::endl;
	std::cout << "        !HighScores!" << std::endl;
	std::cout << "   Score           Name" << std::endl << std::endl;

	std::map<int, std::string>::reverse_iterator Sorter; //reverse iterator to ge the high scores in descending order
	int ScoreStanding = 0;
	for (Sorter = HighScores.rbegin(); Sorter != HighScores.rend(); ++Sorter)
	{
		std::cout << ScoreStanding << ". " << Sorter->first << "     -     " << Sorter->second << "\n";
		ScoreStanding++;
	}
	std::cout << std::endl << "###################################" << std::endl;
	system("pause");
	system("CLS");
	MainMenu(Debugger);

}
void ExitMethod(Debug* Debugger)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	system("CLS");
	std::cout << "############################################################################################################################################" << std::endl << std::endl;
	std::cout << R"(                                                                                                                                                             
                                                                                         ....                                                                  
                                                                               ./%&&&&&&&&&&&&&&&&&&%/.                                                        
                                                                           /&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/                                                    
                                                                       .%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%.                                                
                                                                    ./&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/                                              
                                                                  .&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#.                                           
                                                                .&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.                                         
                                                               ,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*                                        
                                                              #&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%                                       
                                                             &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                      
                                                            &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                     
                                                           &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%                                    
                                                          (&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                   
                                                          &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                   
                                                         #&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                  
                                                         &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%                                  
                                                         /&&&&&&&&%#(////&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/                                  
                                                          &&&#(/////////(&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                   
                                                         /#/////////////(&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&,                                   
                                                     .#####(////////////%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                    
                                                   #########////////////&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                     
                                                *############//////////#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                      
                                             ./###############/////////&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.                                       
                                            (##################(//////%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%                                         
                                          (######################(///(&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                           
                                         ##########################(#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&                                             
                                       *############################  %&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                               
                                      (############################      #&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#                                                  
                                    .############################(           #&&&&&&&&&&&&&&&&&&&&&&&&&&#                                                      
                                   /############################.                 ,%&&&&&&&&&&&&&&%*                                                           
                                  #############################                                                                                                
                                 .###########################/                                                                                                 
                                    ########################                                                                                                    
                             *((((((/ (###################                              
                          .((((((((((((. ###############                              
                         /(((((((((((((((, ###########                                                                                                      
                        ////(((((((((((((((, #######                                                                                                           
                       //  ///((((((((((((((   .##                                         
                           *////(((((((((((/                                                         
                          *////////((((((((                                                              
                         .///////////(((((                                                               
                         ////////    //(/                                                             
                        *///*       ,/*   
                                                                                                                                         
   _|_|_|            _|                        _|      _|            _|      _|_|_|_|            _|                                      
 _|          _|_|    _|    _|_|      _|_|_|  _|_|_|_|        _|_|_|  _|      _|        _|_|_|          _|_|_|  _|_|_|  _|_|      _|_|_|  
 _|        _|_|_|_|  _|  _|_|_|_|  _|_|        _|      _|  _|    _|  _|      _|_|_|    _|    _|  _|  _|    _|  _|    _|    _|  _|    _|  
 _|        _|        _|  _|            _|_|    _|      _|  _|    _|  _|      _|        _|    _|  _|  _|    _|  _|    _|    _|  _|    _|  
   _|_|_|    _|_|_|  _|    _|_|_|  _|_|_|        _|_|  _|    _|_|_|  _|      _|_|_|_|  _|    _|  _|    _|_|_|  _|    _|    _|    _|_|_|  
                                                                                                           _|                            
                                                                                                       _|_|   
                                                                 
   _|_|_|    _|                      _|  _|                      
 _|        _|_|_|_|  _|    _|    _|_|_|        _|_|      _|_|_|  
   _|_|      _|      _|    _|  _|    _|  _|  _|    _|  _|_|      
       _|    _|      _|    _|  _|    _|  _|  _|    _|      _|_|  
 _|_|_|        _|_|    _|_|_|    _|_|_|  _|    _|_|    _|_|_|    
                                                                 
                                                                )" << std::endl;
	std::cout << "This game is proprety of Celestial Enigma Studios." << std::endl;
	std::cout << "Thank you for playing" << std::endl;
	std::cout << std::endl << "############################################################################################################################################" << std::endl;
	//delete all remaining objects:

	delete Debugger;
	Debugger = nullptr;
	//EndScreen:
	system("pause");
}
void GameLoop(Debug* Debugger, Player* Player)
{
	bool IsGameOn = true;
	Update* UpdateManager = new Update;//making pointers
	InputObject* InputManager = new InputObject;
	Events* good = new GoodEvent;
	Events* bad = new BadEvent;
	while (IsGameOn == true)//the actual game loop
	{
		Player->UpdatePlayer(Debugger);//update the player
		bad->EventManager(Player, Debugger);//check for bad event
		good->EventManager(Player, Debugger);//check for good event
		InputManager->InputManager(Debugger, Player);//ask for input
		Player->IncrementDays();
		Player->UpdateInvestments();//ticks 1 day from the investments
		if(Player->CheckWinLose() == true ){ IsGameOn = false; } //checks if you won
		if (InputManager->ReturnInput() == 5) { IsGameOn = false; }//checks if you want to exit
	}
	delete good;//deleting pointers:
	delete bad;
	delete Player;
	delete UpdateManager;
	delete InputManager;
	UpdateManager = nullptr;
	InputManager = nullptr;
	Player = nullptr;
	good = nullptr;
	bad = nullptr;
	ExitMethod(Debugger);
};

