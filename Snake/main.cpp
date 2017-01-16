#include <curses.h>
#include "Game.h" 
#include "Utils.h"
#include "Menu.h"

int main()
{
	Init();
	MainMenu menu(stdscr);
	menu.Work();	

	endwin();
	return 0;
}
