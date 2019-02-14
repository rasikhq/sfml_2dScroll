#include "CApplication.h"
#include "CMenu.h"

extern CApplication Game;

// Menu

CMenu::CMenu() {}

CMenu::~CMenu() {}

void CMenu::draw() {
	for(auto& button : m_Buttons) {
		Game.m_Window->draw(button.getButtonElement());
		Game.m_Window->draw(button.getButtonTextElement());
	}
}

void CMenu::update() {
	for(auto& button : m_Buttons) {
		button.update();
	}
}

void CMenu::addButton(std::string buttonText, BUTTON_ID id) {
	CButton button(this, buttonText, id);
	m_Buttons.push_back(button);
}

const CButton* CMenu::getActiveButton() {
	for(auto& button : m_Buttons) {
		if(button.getActive())
			return &button;
	}
	return nullptr;
}

// Button

CButton::CButton(CMenu *parent, std::string buttonText, BUTTON_ID& id) {
	int y_offset = 0;
	if(parent) {
		m_Parent = parent;
		y_offset = 60 * (int)m_Parent->m_Buttons.size();
	}
	m_ID = id;

	m_ButtonText.setFont(*Game.m_Font);
	m_ButtonText.setString(buttonText);

	m_Button.setSize(sf::Vector2f(
		CButton::BUTTON_SIZE_X >= m_ButtonText.getGlobalBounds().width ? CButton::BUTTON_SIZE_X : CButton::BUTTON_SIZE_X + (m_ButtonText.getGlobalBounds().width - CButton::BUTTON_SIZE_X),
		m_ButtonText.getGlobalBounds().height + (float)m_ButtonText.getCharacterSize()
	));

	m_Button.setPosition(sf::Vector2f(
		(Game.m_WindowResolution.width - m_Button.getGlobalBounds().width) / 2,
		(Game.m_WindowResolution.height + m_Button.getGlobalBounds().top - m_Button.getGlobalBounds().height) / 2 + y_offset
	));
	m_ButtonText.setPosition(sf::Vector2f(
		m_Button.getGlobalBounds().left + (m_Button.getGlobalBounds().width - m_ButtonText.getGlobalBounds().width) / 2,
		m_Button.getGlobalBounds().top + (m_Button.getGlobalBounds().height - m_ButtonText.getGlobalBounds().height - m_ButtonText.getCharacterSize() * 0.5f) / 2 
	));
}

CButton::~CButton() {}

void CButton::update() {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*Game.m_Window);
	if(m_Button.getGlobalBounds().contains((sf::Vector2f)mousePosition)) {
		m_Button.setFillColor(m_Colors["hover"]);
		b_Active = true;
	} else {
		m_Button.setFillColor(m_Colors["default"]);
		b_Active = false;
	}
}

sf::RectangleShape& CButton::getButtonElement() {
	return m_Button;
}

sf::Text& CButton::getButtonTextElement() {
	return m_ButtonText;
}
