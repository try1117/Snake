#pragma once
#include <curses.h>
#include <string>

#include "Map.h"
#include "Snake.h"

class Game
{
public:
	Game(WINDOW *_window);
	~Game();

	void Launch();

private:
	void LoadSettings();
	void DisplayMessage(std::string mes);
	void Refresh();

	WINDOW *window;
	Map *map;
	Snake *snake;
};
