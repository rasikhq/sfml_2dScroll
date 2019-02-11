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

	return true;
}

bool CApplication::Run() {
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

	m_Player->update(dt);
	// Drawing
	// Draw - Environment
	Game_drawEnvironment(dt);
	m_Player->draw();
	m_Rocks->update(dt);
	m_Rocks->draw();

	// Display the window
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
	delete m_Player;
	if(respawn)
		Game_createPlayer();
	else
		return; // TO DO: Go to game over / menu
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
void CApplication::Game_createRocksManager() {
	m_Textures["Rock"].setSmooth(true);
	m_Rocks = new CRocksManager(m_Textures["Rock"]);
}