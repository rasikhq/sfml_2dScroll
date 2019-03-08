#include "CApplication.h"

// Main functions

bool CApplication::LoadTextures() {
	m_Textures["Player"] = sf::Texture();
	m_Textures["Missile"] = sf::Texture();
	m_Textures["Env_BG"] = sf::Texture();
	m_Textures["Rock"] = sf::Texture();

	if(!m_Textures["Player"].loadFromFile("data/textures/player.png"))
		return false;
	if(!m_Textures["Missile"].loadFromFile("data/textures/missile.png"))
		return false;
	if(!m_Textures["Env_BG"].loadFromFile("data/textures/env_bg.png"))
		return false;
	if(!m_Textures["Rock"].loadFromFile("data/textures/meteor.png"))
		return false;
	if(!m_Textures["Health"].loadFromFile("data/textures/health.png"))
		return false;
	return true;
}

bool CApplication::Run_MainMenu() {
	if(m_State != GAME_STATE::MAIN_MENU)
		return true;

	// Clear the window for the next frame
	m_Window->clear();

	// Get the delta time
	sf::Time time_dt = m_Clock.restart();
	float dt = time_dt.asSeconds();
	Game_drawEnvironment(dt);

	// Draw title screen
	m_Window->draw(m_GameTitleScreen);
	m_Window->draw(m_GameCreator);

	// Poll events
	while(m_Window->pollEvent((*m_Event))) {
		if(m_Event->type == sf::Event::MouseButtonPressed && m_Event->mouseButton.button == sf::Mouse::Button::Left) {
			auto button = m_MainMenu.getActiveButton();
			if(!button) {
				continue;
			}
			if(button->getID() == BUTTON_ID::PLAY) {
				Game_destroyPlayer(true);
				m_State = GAME_STATE::RUNNING;
			} else if(button->getID() == BUTTON_ID::QUIT) {
				return false;
			}
		}
	}

	m_MainMenu.update();
	m_MainMenu.draw();

	m_Window->display();
	return true;
}

bool CApplication::Run() {
	if(m_State != GAME_STATE::RUNNING)
		return true;

	// Get the delta time
	sf::Time time_dt = m_Clock.restart();
	float dt = time_dt.asSeconds();

	// Clear the window for the next frame
	m_Window->clear();

	// Poll events
	while(m_Window->pollEvent((*m_Event))) {
		switch(m_Event->type) {
			case sf::Event::Closed:
			return false;
			break;

			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			Event_onGameKeyPress(m_Event->key.code, (m_Event->type == sf::Event::KeyPressed ? true : false));
			break;
		}
		//std::cout << "[EVENT POLL] Event: " << m_Event->type << std::endl;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		Game_movePlayer(dt, -1);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		Game_movePlayer(dt, 1);
	else
		Game_movePlayer(dt);

	m_SoundManager->update();
	m_Player->update(dt);

	if(!m_Player)
		return true;

	// Drawing
	// Draw - Environment {Environment, Score, Player, Rocks}
	Game_drawEnvironment(dt);
	m_ScoreManager->draw();
	m_Player->draw();
	m_Items->update(dt);
	m_Items->draw();

	// Display the window
	m_Window->display();
	return true;
}

bool CApplication::Run_EndMenu() {
	if(m_State != GAME_STATE::END_MENU)
		return true;

	// Clear the window for the next frame
	m_Window->clear();

	m_SoundManager->update();

	// Get the delta time
	sf::Time time_dt = m_Clock.restart();
	float dt = time_dt.asSeconds();
	Game_drawEnvironment(dt);

	// Poll events
	while(m_Window->pollEvent((*m_Event))) {
		if(m_Event->type == sf::Event::MouseButtonPressed && m_Event->mouseButton.button == sf::Mouse::Button::Left) {
			auto button = m_EndMenu.getActiveButton();
			if(!button) {
				continue;
			}
			if(button->getID() == BUTTON_ID::RETRY) {
				Game_destroyPlayer(true);
				m_State = GAME_STATE::RUNNING;
			} else if(button->getID() == BUTTON_ID::MAINMENU) {
				m_State = GAME_STATE::MAIN_MENU;
			}
		}
	}

	m_EndMenu.update();
	m_EndMenu.draw();

	m_Window->display();
	return true;
}

// Event functions

void CApplication::Event_onGameKeyPress(sf::Keyboard::Key& key, bool state) {
	if(key == sf::Keyboard::Key::Space && state) {
		m_Player->shoot();
	}
}

// Game functions

// Game - Player
void CApplication::Game_createPlayer() {
	// Player texture settings
	m_Textures["Player"].setSmooth(true);
	m_Textures["Missile"].setSmooth(true);

	m_Player = new CPlayer(m_Textures["Player"], m_Textures["Missile"]);

	(*m_Player)().setOrigin(sf::Vector2f((*m_Player)().getGlobalBounds().width / 2, (*m_Player)().getGlobalBounds().height / 2));
	(*m_Player)().setPosition(sf::Vector2f((*m_Player)().getGlobalBounds().width + 20.f, m_WindowResolution.height / 2.0f));
}

void CApplication::Game_destroyPlayer(bool respawn) {
	m_SoundManager->playSound("death.ogg");

	m_Items->clear();
	if(respawn)
		Game_createPlayer();
	else
		m_State = GAME_STATE::END_MENU;
}

void CApplication::Game_movePlayer(float& dt, signed short int direction) {
	m_Player->move(dt, direction);
}

// Game - Environment
void CApplication::Game_createEnvironment() {
	// Texture settings
	m_Textures["Env_BG"].setSmooth(true);
	m_Textures["Env_BG"].setRepeated(true);

	m_EnvironmentBackground.setTexture(m_Textures["Env_BG"]);
	m_EnvironmentBackground.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(m_Textures["Env_BG"].getSize().x * 2, m_Textures["Env_BG"].getSize().y)));
	m_EnvironmentBackground.setPosition(0, 0);
}

void CApplication::Game_drawEnvironment(float& dt) {
	// Moving effect
	if(m_EnvironmentBackground.getPosition().x <= 0.0f - m_EnvironmentBackground.getGlobalBounds().width / 2) {
		m_EnvironmentBackground.setPosition(0.0f, 0.0f);
	} else {
		m_EnvironmentBackground.move(-dt * 20.8f, 0.0f);
	}
	
	m_Window->draw(m_EnvironmentBackground);
}

// Game - Rocks Manager
void CApplication::Game_createItemsManager() {
	for(std::unordered_map<std::string, sf::Texture>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++) {
		it->second.setSmooth(true);
	}
	m_Items = new CItemsManager();
}