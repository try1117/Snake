#include <curses.h>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Utils.h"

void Init()
{
	initscr();
	cbreak();
	noecho();
	timeout(0);
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
}

void DisplayMessage(WINDOW *window, std::string mes)
{
	int start_x = window->_begx + (window->_maxx - mes.length()) / 2;
	int start_y = window->_begy + window->_maxy / 2;
	
	WINDOW *meswin = newwin(5, mes.length() + 2, start_y, start_x);
	mvwprintw(meswin, 2, 1, mes.c_str());
	box(meswin, 0, 0);

	//wclear(window);
	wrefresh(meswin);
	timeout(10000);
	getch();
	timeout(0);
}

bool ScoreCmp(const Score &a, const Score &b)
{
	return a.second > b.second;
}

void SortScores(std::vector<Score> &scores)
{
	std::sort(scores.begin(), scores.end(), ScoreCmp);
}

std::string MakeJSONString(std::string a, std::string b)
{
	return "\"" + a + "\"=\"" + b + "\"";
}

std::pair<std::string, std::string> ParseJSONString(std::string s)
{
	s.erase(0, 1);
	s.pop_back();

	int i;
	for (i = (int)s.length() - 1; i >= 2; --i)
		if (s.substr(i - 2, 3) == "\"=\"") {
			std::string name = s.substr(0, i - 2);
			std::string score = s.substr(i + 1, s.length());
			return std::make_pair(name, score);
			break;
		}
	if (i < 2) {
		throw std::exception("String is corrupted");
	}
}

std::vector<Score> ReadScores()
{
	std::ifstream file("leaderboard.txt");
	if (file.fail())
		throw std::exception("Error while opening file \"leaderboard.txt\"");

	std::vector<Score> ans;

	while (!file.eof()) {
		std::string s;
		std::getline(file, s);
		std::pair<std::string, std::string> p;

		try {
			p = ParseJSONString(s);
		} 
		catch(const std::exception e) {
			throw std::exception("File \"leaderboard.txt\" is corrupted");
		}
		ans.push_back(std::make_pair(p.first, std::stoi(p.second)));
	}

	SortScores(ans);
	return ans;
}

void SaveScores(std::vector<Score> &scores)
{
	SortScores(scores);
	std::ofstream file("leaderboard.txt");
	if (file.fail())
		throw std::exception("Can't save to file \"leaderboard.txt\"");

	for (int i = 0; i < scores.size(); ++i) {
		file << "\"" << scores[i].first
			<< "\"=\"" << scores[i].second << "\"";
		if (i + 1 != scores.size())
			file << std::endl;
	}
}

void AddScore(std::vector<Score> &scores, Score &s, int max_cnt)
{
	scores.push_back(s);
	SortScores(scores);
	while (scores.size() > max_cnt)
		scores.pop_back();
}
