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
	void playMusic(std::string, bool, std::string);

	void stopMusic(std::string);

	void update();
private:
	std::unordered_map<std::string, sf::SoundBuffer> m_Buffers;

	std::vector<sf::Sound*> m_Sounds;
	std::unordered_map<std::string, sf::Music*> m_Musics;
};

