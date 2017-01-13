#include <exception>
#include <curses.h>
#include "Game.h"

Game::Game(WINDOW *_window)
{
	window = _window;
	
	try {
		LoadSettings();
	}
	catch (const std::exception e) {
		DisplayMessage(e.what());
		return;
		// TODO: return back to menu
	}

	Refresh();
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
	// show player's info at the top
	// refresh players
	wrefresh(window);
}

void Game::Launch()
{

}
