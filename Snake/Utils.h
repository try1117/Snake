#pragma once
#include <curses.h>
#include <string>

void Init();
void DisplayMessage(WINDOW *window, std::string mes);

typedef std::pair<std::string, int> Score;

std::string MakeJSONString(std::string a, std::string b);
std::pair<std::string, std::string> ParseJSONString(std::string s);

std::vector<Score> ReadScores();
void AddScore(std::vector<Score> &scores, Score &s, int max_cnt);
void SaveScores(std::vector<Score> &scores);

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
