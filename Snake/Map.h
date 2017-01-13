#pragma once
#include <vector>
#include <curses.h>
#include "Utils.h"

enum MapCellState {
	MCS_FREE,
	MCS_FOOD,
	MCS_WALL,
};

class Map
{
public:
	Map(WINDOW *_window);
	~Map();

	int Cols();
	int Rows();

	MapCellState CellState(Cell c);
	void VisitCell(Cell c);

	void Refresh();

	friend class Snake;

private:
	WINDOW *window;
	std::vector<std::vector<MapCellState>> cells;
};