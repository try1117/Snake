#pragma once
#include <curses.h>
#include <string>

class Game
{
public:
	Game(WINDOW *window);
	~Game();

	void Launch();

private:
	void LoadSettings();
	void DisplayMessage(std::string mes);
};