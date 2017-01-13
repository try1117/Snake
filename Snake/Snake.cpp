#include <set>
#include "Snake.h"

Snake::Snake(Map *_map) : map(_map)
{
	direction = DIR_RIGHT;
	body.push_front(Cell(0, 0));
	body.push_front(Cell(0, 1));
	body.push_front(Cell(0, 2));
	body.push_front(Cell(0, 3));
}

Snake::~Snake()
{

}

SnakeState Snake::Move()
{
	if (food.count(body.back())) {
		food.erase(food.find(body.back()));
	}
	else {
		body.pop_back();
	}

	int nrow = map->Rows() + body.front().row + drow[direction];
	int ncol = map->Cols() + body.front().col + dcol[direction];

	body.push_front(Cell(nrow % map->Rows(), ncol % map->Cols()));

	if (map->CellState(body.front()) == MCS_FOOD) {
		map->VisitCell(body.front());
		food.insert(body.front());
	}

	for (auto it = next(body.begin()); it != body.end(); ++it) {
		if (*it == body.front())
			return ST_LOST;
	}

	if (nrow < map->Rows() || nrow == 2 * map->Rows() ||
		ncol < map->Cols() || ncol == 2 * map->Cols())
	{
		return ST_INTERSECTED;
	}

	return ST_NORMAL;
}

void Snake::Turn(Direction _direction)
{
	// we can't turn to the opposite direction
	if (_direction != (direction + 2) % 4)
		direction = _direction;
}


void Snake::Refresh()
{
	for (auto it = body.begin(); it != body.end(); ++it) {
		mvwaddch(map->window, 1 + it->row, 1 + it->col, '@');
	}
}
