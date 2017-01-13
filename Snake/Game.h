#pragma once
#include <curses.h>
#include <string>

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
};
