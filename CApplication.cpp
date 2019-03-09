#include "CApplication.h"

// Main functions

bool CApplication::LoadTextures() {
	if(!m_Textures["Player"].loadFromFile("data/textures/player.png"))
		return false;
	if(!m_Textures["Missile"].loadFromFile("data/textures/missile.png"))
		return false;
	// Levels
	if(!m_Textures["level_1"].loadFromFile("data/textures/levels/level_1.png"))
		return false;
	if(!m_Textures["level_2"].loadFromFile("data/textures/levels/level_2.png"))
		return false;
	// Items
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
void CApplication::Game_createEnvironment(std::string envTexture, bool setSmooth, bool setRepeated) {
	// Texture settings
	m_Textures[envTexture].setSmooth(setSmooth);
	m_Textures[envTexture].setRepeated(setRepeated);

	m_EnvironmentBackground.setTexture(m_Textures[envTexture]);
	//m_EnvironmentBackground.setScale(m_Window->getView().getSize().x / m_EnvironmentBackground.getLocalBounds().width, m_Window->getView().getSize().y / m_EnvironmentBackground.getLocalBounds().height);
	m_EnvironmentBackground.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(m_Window->getSize().x*2, m_Window->getSize().y)));
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
	m_Textures["Rock"].setSmooth(true);
	m_Textures["Health"].setSmooth(true);
	m_Items = new CItemsManager();
}