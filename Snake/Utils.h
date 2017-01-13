#pragma once

void Init();

struct Cell {
	int col, row;
	Cell(int r, int c) : row(r), col(c) {}

	bool operator==(const Cell &a) const {
		return row == a.row && col == a.col;
	}

	bool operator<(const Cell &a) const {
		if (row == a.row)
			return col < a.col;
		return row < a.row;
	}
};