#include <curses.h>
#include "Game.h" 
#include "Utils.h"

int main()
{
	int a = -1 % 10;


	Init();
	Game game(newwin(LINES, COLS, 0, 0));
	game.Launch();
	getchar();

	endwin();
	return 0;
}
