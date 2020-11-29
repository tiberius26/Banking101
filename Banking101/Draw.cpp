#include "Draw.h"

void Draw::DrawScene(Debug* Debugger,  Player* Player, int DrawState)
{
	DrawReport(Debugger, Player);
	DrawBank(Debugger, Player->ReturnLevel());
	if (DrawState == 1) { DrawWin(); }
	else if (DrawState == 2) { DrawLose(); }
}
void Draw::DrawWin()
{

}
void Draw::DrawLose()
{

}

