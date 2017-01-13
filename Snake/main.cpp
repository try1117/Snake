#include <curses.h>
#include "Game.h" 
#include "Utils.h"

int main()
{
	Init();
	Game game(newwin(LINES, COLS, 0, 0));
	getchar();

	endwin();
	return 0;
}
