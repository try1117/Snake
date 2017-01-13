#include <exception>
#include "Game.h"

Game::Game(WINDOW *window)
{
	try {
		LoadSettings();
	}
	catch (const std::exception e) {
		DisplayMessage(e.what());
	}
}

Game::~Game()
{
	// free something
}

void Game::LoadSettings()
{

}

void Game::DisplayMessage(std::string mes)
{
	
}

void Game::Launch()
{

}