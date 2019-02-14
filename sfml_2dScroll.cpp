#define GAME_VERSION 01
#define PROJECT "Space Shooter"
#define PROJECT_AUTHOR "DizzasTeR"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

#include "CApplication.h"

CApplication Game(sf::VideoMode(800, 600), PROJECT " - " PROJECT_AUTHOR);

int main(int argsc, char** args)
{
	if(argsc > 1) {
		std::stringstream ss;
		ss << PROJECT << " - " << args[1] << (argsc > 2 ? " " + std::string(args[2]) : "");
		Game.m_Window->setTitle(ss.str());
	}
	srand((unsigned int)time(NULL));

	/* Window settings */
	Game.m_Window->setKeyRepeatEnabled(false);
	Game.m_Window->setFramerateLimit(60);

	/* Game loop */
	while(Game.Process())
		continue;
	return 0;
}
