#include "consoleUtil.h"
#include <cstdlib>
#include <winbase.h>

ConsoleUtil::ConsoleUtil()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = FALSE;
	info.dwSize = 100;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hOutput, &info);
}

ConsoleUtil::~ConsoleUtil()
{
}

void ConsoleUtil::changeColour(WORD theColour)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, theColour);
}

void ConsoleUtil::gotoXY(int x, int y)
{
	COORD coord = { x, y };													// Initialize the coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);		// Set the position
}

void ConsoleUtil::clearscreen()
{
	system("cls");
}