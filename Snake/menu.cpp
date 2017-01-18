#include <algorithm>
#include "Menu.h"
#include "Utils.h"
#include "Game.h"

Label::Label(std::string _text) : text(_text)
{
	selectable = false;
}

Label::~Label()
{

}

std::string Label::Text()
{
	return text;
}

bool Label::Selectable()
{
	return selectable;
}

Item::Item(std::string text, std::function<void(void)> _onclick) 
	: Label(text), onclick(_onclick)
{
	selectable = true;
}

Item::~Item()
{

}

void Item::Click()
{
	onclick();
}

ComboBox::ComboBox(std::string text, std::vector<std::string> _content)
	: Label(text), content(_content)
{
	selectable = true;
	current = 0;
	if (content.empty())
		throw std::exception("Content is empty");
}

ComboBox::~ComboBox()
{

}

std::string ComboBox::Value()
{
	return content[current];
}

std::string ComboBox::Text()
{
	return Label::Text() + ' ' + Value();
}

void ComboBox::Click()
{
	current = (current + 1) % content.size();
}

Edit::Edit(std::string text, std::function<void(std::string)> _onclick)
	: Label(text)
{
	onclick = _onclick;
	value = "";
	selectable = true;
}

std::string Edit::Value()
{
	return value;
}

std::string Edit::Text()
{
	return Label::Text() + ' ' + Value();
}

void Edit::EraseSymbol()
{
	if (!value.empty())
		value.pop_back();
}

void Edit::Click()
{
	onclick(value);
}

void Edit::AddSymbol(char c)
{
	value += c;
}

Input::Input(std::string text, int &_key)
	: Label(text), key(_key)
{
	state = IS_NORMAL;
	selectable = true;
}

std::string Input::Value()
{
	if (state == IS_NORMAL)
		return keyname(key);
	return "...";
}

std::string Input::Text()
{
	return Label::Text() + ' ' + Value();
}

void Input::PassKey(int _key)
{
	key = _key;
	Click();
}

void Input::Click()
{
	state = InputState(!state);
}

Menu::Menu(WINDOW *_window) : window(_window)
{
	focused_item = -1;
}

Menu::~Menu()
{
	for (int i = 0; i < items.size(); ++i)
		delete items[i];
}

void Menu::AddItem(Label *it)
{
	items.push_back(it);
}

int Menu::SearchSelectable(int index, int sign)
{
	int i;
	for (i = 0; i < items.size(); ++i)
		if (items[i]->Selectable())
			break;

	// there's no selectable items
	if (i == items.size())
		return -1;

	for (int i = 1; i <= items.size(); ++i) {
		int cur = (items.size() + index + i * sign) % items.size();
		if (items[cur]->Selectable())
			return cur;
	}
}

void Menu::Refresh()
{
	wclear(window);

	int row = (window->_maxy - items.size()) / 2;
	int col = (window->_maxx - items[0]->Text().length()) / 2;

	for (int i = 0; i < items.size(); ++i) {
		if (i == focused_item)
			attrset(A_UNDERLINE);
		else
			attrset(A_NORMAL);

		mvwaddstr(window, row + i, col, items[i]->Text().c_str());
		attrset(A_NORMAL);
	}
	wrefresh(window);
}

void Menu::Work()
{
	while (true) {
		Refresh();
		
		int key = getch();
		switch (key) {
		case KEY_DOWN:
			focused_item = SearchSelectable(focused_item, +1);
			break;
		case KEY_UP:
			focused_item = SearchSelectable(focused_item, -1);
			break;
		case 10:
			if (focused_item != -1)
				dynamic_cast<IClickable*>(items[focused_item])->Click();
			break;
		case 27:
			return;
		}
	}
}

MainMenu::MainMenu(WINDOW *window) : Menu(window)
{
	std::function<void(void)> play = [window]() { GameMenu(window).Work(); };
	std::function<void(void)> options = [window]() { Options(window).Work(); };
	std::function<void(void)> leaderboard = [window]() { Leaderboard(window).Work(); };
	std::function<void(void)> quit = []() {exit(0); };
	
	AddItem(new Item("Play", play));
	AddItem(new Item("Options", options));
	AddItem(new Item("Leaderboard", leaderboard));
	AddItem(new Item("Quit", quit));

	focused_item = SearchSelectable(-1, +1);
}

GameMenu::GameMenu(WINDOW *window) : Menu(window)
{
	AddItem(new Label("Choose options"));
	AddItem(new Label(""));

	std::vector<std::string> speed_params;
	speed_params.push_back("increasing");
	for (int i = 1; i < 10; ++i) {
		speed_params.push_back(std::to_string(i));
	}

	std::vector<std::string> border_params = { "on", "off" };

	ComboBox *speed_edit = new ComboBox("Speed", speed_params);
	ComboBox *border_edit = new ComboBox("Border", border_params);

	AddItem(speed_edit);
	AddItem(border_edit);

	std::function<void(void)> play_game = [window, speed_edit, border_edit]() {
		Game(window, GameParams(speed_edit->Value(), border_edit->Value())).Launch();
	};

	AddItem(new Item("ok", play_game));
	focused_item = SearchSelectable(-1, +1);
}

Leaderboard::Leaderboard(WINDOW *window) : Menu(window)
{
	AddItem(new Label("Leaderboard"));
	AddItem(new Label(""));
	
	try {
		std::vector<std::pair<std::string, int>> scores = ReadScores();

		int maxlen = 0;
		for (int i = 0; i < scores.size(); ++i) {
			maxlen = std::max(maxlen, (int)scores[i].first.length());
		}

		int gap = 3;
		for (int i = 0; i < scores.size(); ++i) {
			std::string name = scores[i].first + std::string(gap + maxlen - scores[i].first.length(), ' ');
			std::string score = std::to_string(scores[i].second);
			AddItem(new Label(name + score));
		}
	}
	catch (const std::exception e) {
		DisplayMessage(window, e.what());
	}
}

GameOverMenu::GameOverMenu(WINDOW *window, int score, std::function<void(std::string)> onclick)
	: Menu(window)
{
	AddItem(new Label("Your result is " + std::to_string(score)));
	AddItem(new Edit("Enter your name: ", onclick));
	focused_item = SearchSelectable(-1, +1);
}

void GameOverMenu::Work()
{
	Edit *e;
	e = dynamic_cast<Edit*>(items[focused_item]);

	while (true) {
		Refresh();

		int key = getch();
		switch (key) {
		//ENTER
		case 10:
			e->Click();
			return;
		// ESCAPE
		case 27:
			return;
		// BACKSPACE
		case 8:
			e->EraseSymbol();
			break;
		default:
			//isalnum(key)
			if (32 <= key && key <= 127)
				e->AddSymbol(key);
			break;
		}
	}
}

Options::Options(WINDOW *window) : Menu(window)
{
	AddItem(new Label("Movement options"));

	controls = Game::LoadSettings();
	AddItem(new Input("UP   ", controls.up));
	AddItem(new Input("DOWN ", controls.down));
	AddItem(new Input("LEFT ", controls.left));
	AddItem(new Input("RIGHT", controls.right));

	//AddItem(new Label(""));
	//AddItem(new Item("save", [controls]() { Game::SaveSettings(controls); }));

	focused_item = SearchSelectable(-1, +1);
}

void Options::Work()
{
	bool toogled = false;

	while (true) {
		Refresh();
		int key = getch();

		if (toogled && key == 10) {
			dynamic_cast<IClickable*>(items[focused_item])->Click();
			toogled = false;
			continue;
		}

		if (toogled && key != -1) {
			dynamic_cast<Input*>(items[focused_item])->PassKey(key);
			toogled = false;
			continue;
		}

		switch (key) {
		case KEY_DOWN:
			focused_item = SearchSelectable(focused_item, +1);
			break;
		case KEY_UP:
			focused_item = SearchSelectable(focused_item, -1);
			break;
		case 10:
			if (focused_item != -1) {
				dynamic_cast<IClickable*>(items[focused_item])->Click();
				toogled = !toogled;
			}
			break;
		case 27:
			Game::SaveSettings(controls);
			return;
		}
	}
}
