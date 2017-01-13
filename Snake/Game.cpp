#include <exception>
#include <curses.h>
#include <thread>
#include <chrono>

#include "Game.h"

Game::Game(WINDOW *_window)
{
	window = _window;
	map = new Map(window);
	snake = new Snake(map);

	try {
		LoadSettings();
	}
	catch (const std::exception e) {
		DisplayMessage(e.what());
		return;
		// TODO: return back to menu
	}
}

Game::~Game()
{
	delwin(window);
}

void Game::LoadSettings()
{
	//throw std::exception("Can't find config file");
}

void Game::DisplayMessage(std::string mes)
{
	int center_x = (window->_maxx - mes.length()) / 2;
	int center_y = window->_maxy / 2;
	mvwprintw(window, center_y, center_x, mes.c_str());
	wrefresh(window);
}

void Game::Refresh()
{
	box(window, 0, 0);
	map->Refresh();
	snake->Refresh();
	wrefresh(window);
}

void Game::Launch()
{
	for (int i = 1; i < 30; ++i) {
		Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		snake->Move();
		if (i % 10 == 0)
			snake->Turn(DIR_UP);
		if (i % 19 == 0)
			snake->Turn(DIR_LEFT);
	}
}
