#include <curses.h>
#include "Utils.h"

void Init()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
}
