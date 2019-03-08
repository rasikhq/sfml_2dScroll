#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

#include "CPlayer.h"
#include "CMenu.h"
#include "CSoundManager.h"
#include "CItemsManager.h"
#include "CScoreManager.h"

enum GAME_STATE {
	MAIN_MENU,
	RUNNING,
	END_MENU
};

struct CApplication {
	CApplication(const sf::VideoMode& WindowResolution, const std::string& WindowTitle) 
		: m_WindowResolution(WindowResolution)
		, m_WindowTitle(WindowTitle)
	{
		m_Window = new sf::RenderWindow(WindowResolution, WindowTitle);
		m_Event  = new sf::Event();
		m_Font   = new sf::Font();

		if(!m_Font->loadFromFile("data/fonts/arial.ttf"))
			APP_EXIT_FAILURE("Could not load font!");

		if(!LoadTextures())
			APP_EXIT_FAILURE("Could not load textures!");

		// Create managers
		m_SoundManager = new CSoundManager();
		m_ScoreManager = new CScoreManager();

		// Set up menus
		m_MainMenu.addButton("Play", BUTTON_ID::PLAY);
		m_MainMenu.addButton("Quit", BUTTON_ID::QUIT);
		m_MainMenu.m_Buttons.at(1).m_Colors["hover"] = sf::Color::Red;

		m_EndMenu.addButton("Try Again", BUTTON_ID::RETRY);
		m_EndMenu.addButton("Menu", BUTTON_ID::MAINMENU);
		m_EndMenu.m_Buttons.at(1).m_Colors["hover"] = sf::Color::Yellow;

		// Set up game
		Game_createEnvironment();
		Game_createPlayer();
		Game_createItemsManager();

		m_SoundManager->createAudio("death.ogg");
		m_SoundManager->createAudio("hit.ogg");

		//m_SoundManager->playMusic("bg.ogg", true);

		// Title screen
		m_GameTitleScreen.setFont(*m_Font);
		m_GameTitleScreen.setString("Space Shooter");
		m_GameTitleScreen.setCharacterSize(50);
		m_GameTitleScreen.setPosition(m_WindowResolution.width*0.5f - m_GameTitleScreen.getGlobalBounds().width*0.5f,
									  m_WindowResolution.height*0.5f - m_GameTitleScreen.getGlobalBounds().width*0.85f);

		m_GameCreator.setFont(*m_Font);
		m_GameCreator.setString("( by Rasikh Qadeer )");
		m_GameCreator.setCharacterSize(20);
		m_GameCreator.setStyle(sf::Text::Italic);
		m_GameCreator.setPosition(m_WindowResolution.width*0.5f - m_GameTitleScreen.getGlobalBounds().width*0.1f,
									  m_WindowResolution.height*0.5f - m_GameTitleScreen.getGlobalBounds().width*0.65f);
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
		switch(m_State) {
			case GAME_STATE::MAIN_MENU:
				return Run_MainMenu();
			break;

			case GAME_STATE::RUNNING:
				return Run();
			break;

			case GAME_STATE::END_MENU:
				return Run_EndMenu();
			break;

			default:
				return false;
			break;
		}
	}

	// Main functions
	bool LoadTextures();
	bool Run_MainMenu();
	bool Run();
	bool Run_EndMenu();

	// Event functions
	void Event_onGameKeyPress(sf::Keyboard::Key&, bool);

	// Game functions
	void Game_createPlayer();
	void Game_destroyPlayer(bool = false);
	void Game_movePlayer(float&, signed short int = 0);

	void Game_createEnvironment();
	void Game_drawEnvironment(float&);

	void Game_createItemsManager();

	// Member variables

	const sf::VideoMode m_WindowResolution;
	const std::string m_WindowTitle;

	sf::Clock m_Clock;
	sf::RenderWindow* m_Window;
	sf::Event* m_Event;
	sf::Font* m_Font;

	std::unordered_map<std::string, sf::Texture> m_Textures;

	CPlayer* m_Player;
	CSoundManager* m_SoundManager;
	CItemsManager* m_Items;
	CScoreManager* m_ScoreManager;

	CMenu m_MainMenu;
	CMenu m_EndMenu;

	sf::Text m_GameTitleScreen;
	sf::Text m_GameCreator;
	sf::Sprite m_EnvironmentBackground;

	GAME_STATE m_State = GAME_STATE::MAIN_MENU;
	bool b_ExitFlag = false;
};