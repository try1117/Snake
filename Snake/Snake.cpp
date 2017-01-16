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

std::vector<SnakeState> Snake::Move()
{
	std::vector<SnakeState> ans;

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
		ans.push_back(ST_FOOD);
	}

	for (auto it = next(body.begin()); it != body.end(); ++it) {
		if (*it == body.front())
			ans.push_back(ST_INTERSECTION);
	}

	if (nrow < map->Rows() || nrow == 2 * map->Rows() ||
		ncol < map->Cols() || ncol == 2 * map->Cols())
	{
		ans.push_back(ST_BORDER);
	}

	return ans;
}

void Snake::Turn(Direction _direction)
{
	// we can't turn to the opposite direction
	if (_direction != DIR_NONE && _direction != (direction + 2) % 4)
		direction = _direction;
}


void Snake::Refresh()
{
	for (auto it = body.begin(); it != body.end(); ++it) {
		mvwaddch(map->window, it->row, it->col, '@');
	}
}
