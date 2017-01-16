#include <algorithm>
#include "Map.h"

Map::Map(WINDOW *_window, int rows, int cols) : window(_window)
{
	for (int i = 0; i < rows; ++i)
		cells.push_back(std::vector<MapCellState>(cols, MCS_FREE));

	int food_cnt = std::max(cols, rows) / 15;
	for (int i = 0; i < food_cnt; ++i) {
		GenerateFood();
	}
}

Map::~Map()
{

}

int Map::Rows()
{
	return cells.size();
}

int Map::Cols()
{
	// assuming that our field is rectangular
	return cells[0].size();
}

MapCellState Map::CellState(Cell c)
{
	return cells[c.row][c.col];
}

void Map::VisitCell(Cell c)
{
	if (cells[c.row][c.col] == MCS_FOOD) {
		cells[c.row][c.col] = MCS_FREE;
		GenerateFood();
	}
}

void Map::GenerateFood()
{
	int row = rand() % Rows();
	int col = rand() % Cols();

	while (cells[row][col] != MCS_FREE) {
		row = rand() % Rows();
		col = rand() % Cols();
	}

	cells[row][col] = MCS_FOOD;
}

void Map::Refresh()
{
	for (int i = 0; i < cells.size(); ++i)
		for (int j = 0; j < cells[i].size(); ++j) {
			if (cells[i][j] == MCS_FREE) {
				mvwaddch(window, i, j, ' ');
			}
			if (cells[i][j] == MCS_FOOD) {
				mvwaddch(window, i, j, '*');
			}
		}
}
