#pragma once
#include <string>
#include <vector>
#include <functional>
#include <curses.h>

class Label {
public:
	Label(std::string _text);
	~Label();
	virtual std::string Text();
	bool Selectable();

protected:
	bool selectable;
	std::string text;
};

class IClickable {
public:
	virtual void Click() = 0;
};

class Item : public Label, public IClickable {
public:
	Item(std::string text, std::function<void(void)> _onlick);
	~Item();
	void Click();

protected:
	std::function<void(void)> onclick;
};

class ComboBox : public Label, public IClickable {
public:
	ComboBox(std::string text, std::vector<std::string> _content);
	~ComboBox();
	
	std::string Value();
	std::string Text();
	void Click();

protected:
	int current;
	std::vector<std::string> content;
};

class Edit : public Label, public IClickable {
public:
	Edit(std::string text, std::function<void(std::string)> onlick);

	std::string Value();
	std::string Text();
	void Click();
	void EraseSymbol();
	void AddSymbol(char c);

private:
	std::string value;
	std::function<void(std::string)> onclick;
};

class Menu {
public:
	Menu(WINDOW *_window);
	~Menu();

	void AddItem(Label *it);
	void Refresh();
	void Work();

protected:
	// return -1 if there's not selectable items
	int SearchSelectable(int index, int sign);

	WINDOW *window;
	std::vector<Label*> items;
	int focused_item;
};

class MainMenu : public Menu {
public:
	MainMenu(WINDOW *window);
};

class GameMenu : public Menu {
public:
	GameMenu(WINDOW *window);
};

class Leaderboard : public Menu {
public:
	Leaderboard(WINDOW *window);
};

class Options : public Menu {
public:
	Options(WINDOW *window);
};

class GameOverMenu : public Menu {
public:
	GameOverMenu(WINDOW *window, int score, std::function<void(std::string)> _onlick);
	void Work();
};
