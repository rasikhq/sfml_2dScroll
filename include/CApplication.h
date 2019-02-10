#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

#include "CPlayer.h"
#include "CRocksManager.h"

struct CApplication {
	CApplication(const sf::VideoMode& WindowResolution, const std::string& WindowTitle) 
		: m_WindowResolution(WindowResolution)
		, m_WindowTitle(WindowTitle)
	{
		m_Window = new sf::RenderWindow(WindowResolution, WindowTitle);
		m_Event = new sf::Event();
		m_Font = new sf::Font();

		if(!m_Font->loadFromFile("data/fonts/arial.ttf"))
			APP_EXIT_FAILURE("Could not load font!");

		if(!LoadTextures())
			APP_EXIT_FAILURE("Could not load textures!");

		Game_createEnvironment();
		Game_createPlayer();
		Game_createRocksManager();
	}

	~CApplication() {
		delete m_Window;
		delete m_Event;
		delete m_Font;
	}

	void APP_EXIT_FAILURE(const std::string error) {
		std::cout << "[ERROR] " << error << std::endl;
		b_ExitFlag = true;
	}

	bool Process() {
		if(b_ExitFlag)
			return false;
		return Run();
	}

	// Main functions
	bool LoadTextures();
	bool Run();

	// Event functions
	void Event_onGameKeyPress(sf::Keyboard::Key&, bool);

	// Game functions
	void Game_createPlayer();
	void Game_movePlayer(float&, signed short int = 0);

	void Game_createEnvironment();
	void Game_drawEnvironment(float&);

	void Game_createRocksManager();

	// Member variables

	const sf::VideoMode m_WindowResolution;
	const std::string m_WindowTitle;

	sf::Clock m_Clock;
	sf::RenderWindow* m_Window;
	sf::Event* m_Event;
	sf::Font* m_Font;

	std::unordered_map<std::string, sf::Texture> m_Textures;

	CPlayer* m_Player;
	CRocksManager* m_Rocks;
	sf::Sprite m_EnvironmentBackground;

	bool b_ExitFlag = false;
};