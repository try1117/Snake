#pragma once
#include <curses.h>
#include <string>

#include "Map.h"
#include "Snake.h"

struct GameParams {
	std::string speed_param;
	std::string border_param;

	GameParams(std::string sp, std::string bp);
};

struct Controls {
public:
	int up, down;
	int left, right;

	Controls() {};
	Controls(int u, int d, int l, int r)
		: up(u), down(d), left(l), right(r) {};

	Direction operator[](int key);
};

class Game
{
public:
	Game(WINDOW *_window, GameParams gp);
	~Game();

	void Launch();
	void GameOver(Score sc);
	static Controls LoadSettings();
	static void SaveSettings(Controls controls);
	
private:
	void Refresh();

	WINDOW *window, *game_wind, *box_wind;
	Map *map;
	Snake *snake;
	Controls controls;

	const int rows = 20;
	const int cols = 40;

	const int start_period = 300;
	const int min_period = 45;
	const int level_step = 28;
	const int food_step = 10;

	int period;
	bool speed_increase;
	bool solid_borders;
};
