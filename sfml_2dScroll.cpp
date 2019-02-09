#define GAME_VERSION 01
#define PROJECT "2D Scroller"
#define PROJECT_AUTHOR "Rasikh Qadeer"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#include "CApplication.h"

CApplication Game(sf::VideoMode(800, 600), PROJECT " - " PROJECT_AUTHOR);

int main()
{
	srand((unsigned int)time(NULL));

	/* Window settings */
	Game.m_Window->setKeyRepeatEnabled(false);
	Game.m_Window->setFramerateLimit(60);

	/* Game loop */
	while(Game.Process())
		continue;
	return 0;
}
