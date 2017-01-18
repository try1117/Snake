#include <exception>
#include <curses.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>

#include "Game.h"
#include "Utils.h"
#include "Menu.h"

GameParams::GameParams(std::string sp, std::string bp)
	: speed_param(sp), border_param(bp)
{

}

Direction Controls::operator[](int key)
{
	if (key == up) return DIR_UP;
	if (key == down) return DIR_DOWN;
	if (key == left) return DIR_LEFT;
	if (key == right) return DIR_RIGHT;
	return DIR_NONE;
}

Game::Game(WINDOW *_window, GameParams params)
{
	game_wind = newwin(rows, cols, 1, 1);
	box_wind = newwin(rows + 2, cols + 2, 0, 0);
	window = _window;

	wclear(window);
	wrefresh(window);

	wborder(box_wind, '|', '|', '-', '-', 0, 0, 0, 0);
	box(box_wind, 0, 0);
	wrefresh(box_wind);	

	map = new Map(game_wind, rows, cols);
	snake = new Snake(map);

	period = start_period;
	if (params.speed_param == "increasing")
		speed_increase = true;
	else {
		speed_increase = false;
		period -= level_step * std::stoi(params.speed_param);
	}

	solid_borders = (params.border_param == "on");

	try {
		controls = LoadSettings();
	}
	catch (const std::exception e) {
		DisplayMessage(window, e.what());
		return;
		// TODO: return back to menu
	}
}

Game::~Game()
{
	delwin(game_wind);
	delwin(box_wind);
}

Controls Game::LoadSettings()
{
	Controls controls;
	std::ifstream file("config.txt");
	if (file.fail()) {
		throw std::exception("Error while opening file \"config.txt\"");
	}

	while (!file.eof()) {
		std::string s;
		std::getline(file, s);
		std::pair<std::string, std::string> p = ParseJSONString(s);
		int val = std::stoi(p.second);

		if (p.first == "UP") controls.up = val;
		if (p.first == "DOWN") controls.down = val;
		if (p.first == "LEFT") controls.left = val;
		if (p.first == "RIGHT") controls.right = val;
	}

	return controls;
}

void Game::SaveSettings(Controls controls)
{
	std::ofstream file("config.txt");
	if (file.fail()) {
		throw std::exception("Error while creating file \"config.txt\"");
	}

	file <<MakeJSONString("UP", std::to_string(controls.up)) << std::endl
		<< MakeJSONString("DOWN", std::to_string(controls.down)) << std::endl
		<< MakeJSONString("LEFT", std::to_string(controls.left)) << std::endl
		<< MakeJSONString("RIGHT", std::to_string(controls.right));
}

void Game::Refresh()
{
	map->Refresh();
	snake->Refresh();
	wrefresh(game_wind);
}

void Game::Launch()
{
	std::vector<SnakeState> st;
	int score = 0;

	while (true) {
		Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(period));
		
		int key = getch();
		snake->Turn(controls[key]);
		st = snake->Move();

		for (int i = 0; i < st.size(); ++i) {
			if (st[i] == ST_FOOD) {
				period = std::max(min_period, period - speed_increase * food_step);
				++score;
			}
			if (st[i] == ST_INTERSECTION || st[i] == ST_BORDER && solid_borders) {
				GameOverMenu gom(window, score, [this, score](std::string name) {GameOver(make_pair(name, score)); });
				gom.Work();
				return;
			}
		}
	}
}

void Game::GameOver(Score sc)
{
	std::vector<Score> scores = ReadScores();
	AddScore(scores, sc, 10);
	SaveScores(scores);
}
