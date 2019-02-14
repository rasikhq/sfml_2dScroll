#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <functional>

enum BUTTON_ID {
	PLAY,
	QUIT,
	RETRY,
	MAINMENU
};

class CButton;
class CMenu;

class CMenu
{
public:
	CMenu();
	~CMenu();

	void draw();
	void update();
	void addButton(std::string, BUTTON_ID);
	const CButton* getActiveButton();

	std::vector<CButton> m_Buttons;
};

class CButton {
public:
	static constexpr float BUTTON_SIZE_X = 150.0f;
	static constexpr float  BUTTON_SIZE_Y = 50.0f;

	CButton(CMenu*, std::string, BUTTON_ID&);
	~CButton();

	void update();
	void setActive(bool state) { b_Active = state; }
	bool getActive() const { return b_Active; }
	BUTTON_ID getID() const { return m_ID; }

	sf::RectangleShape& getButtonElement();
	sf::Text& getButtonTextElement();

	std::unordered_map<std::string, sf::Color> m_Colors =
	{
		{"default", sf::Color::Color(50, 50, 50, 255)},
		{"hover", sf::Color::Green}
	};
private:
	CMenu* m_Parent = nullptr;

	sf::RectangleShape m_Button;
	sf::Text m_ButtonText;
	std::vector<std::string> m_Options;

	BUTTON_ID m_ID;
	bool b_Active = false;
};