#pragma once
#include <list>
#include <set>
#include "Map.h"
#include "Utils.h"

enum SnakeState {
	ST_NORMAL,
	ST_BORDER,
	ST_FOOD,
	ST_INTERSECTION,
};

enum Direction {
	DIR_NONE = -1,
	DIR_RIGHT = 0,
	DIR_UP = 1,
	DIR_LEFT = 2,
	DIR_DOWN = 3,
};

const int drow[] = { 0, -1, 0, 1 };
const int dcol[] = { 1, 0, -1, 0 };

class Snake
{
public:
	Snake(Map *_map);
	~Snake();

	std::vector<SnakeState> Move();
	void Turn(Direction _direction);

	void Refresh();

private:
	Map *map;
	Direction direction;
	std::list<Cell> body;
	std::multiset<Cell> food;
};
