#include "Map.h"

Map::Map(WINDOW *_window) : window(_window)
{
	int size = 10;
	cells.resize(size);

	for (int i = 0; i < size; ++i) {
		//cells.push_back(std::vector<MapCellState>(size, MCS_FREE));

		for (int j = 0; j < size; ++j)
			if (rand() % 100 > 70)
				cells[i].push_back(MCS_FOOD);
			else
				cells[i].push_back(MCS_FREE);
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
	}
}

void Map::Refresh()
{
	for (int i = 0; i < cells.size(); ++i)
		for (int j = 0; j < cells[i].size(); ++j) {
			if (cells[i][j] == MCS_FREE) {
				mvwaddch(window, 1 + i, 1 + j, '.');
			}
			if (cells[i][j] == MCS_FOOD) {
				mvwaddch(window, 1 + i, 1 + j, '*');
			}
		}
}
