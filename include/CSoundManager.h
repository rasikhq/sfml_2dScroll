#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	void createAudio(std::string);

	void playSound(std::string, bool = false);
	void playMusic(std::string, bool = false);

	void update();
private:
	std::unordered_map<std::string, sf::SoundBuffer> m_Buffers;

	std::vector<sf::Sound> m_Sounds;
	std::vector<sf::Music*> m_Musics;
};

