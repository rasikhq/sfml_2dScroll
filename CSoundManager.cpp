#include "CSoundManager.h"

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
}

void CSoundManager::createAudio(std::string fileName) {
	m_Buffers[fileName] = sf::SoundBuffer();
	if(!m_Buffers[fileName].loadFromFile("data/sounds/" + fileName)) {
		std::cout << "[CSoundManager] Unable to load sound: " << fileName << std::endl;
		m_Buffers.erase(fileName);
		return;
	}
}

void CSoundManager::playSound(std::string fileName, bool loop) {
	if(m_Buffers.find(fileName) == m_Buffers.end())
		createAudio(fileName);

	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(m_Buffers[fileName]);
	sound->setLoop(loop);
	sound->play();
	m_Sounds.push_back(sound);
}

void CSoundManager::playMusic(std::string fileName, bool loop, std::string soundID) {
	sf::Music* music = new sf::Music();
	if(!music->openFromFile("data/sounds/" + fileName)) {
		std::cout << "[CSoundManager] Unable to load music: " << fileName << std::endl;
		delete music;
		return;
	}
	music->setLoop(loop);
	music->play();
	m_Musics[soundID] = music;
}

void CSoundManager::stopMusic(std::string soundID) {
	if(m_Musics.find(soundID) == m_Musics.end())
		return;

	delete m_Musics[soundID];
	m_Musics.erase(soundID);
}

void CSoundManager::update() {
	std::vector<sf::Sound*>::const_iterator sound_it;
	for(sound_it = m_Sounds.begin(); sound_it != m_Sounds.end();) {
		if((*sound_it)->getStatus() == sf::Sound::Stopped) {
			delete (*sound_it);
			sound_it = m_Sounds.erase(sound_it);
		} else {
			sound_it++;
		}
	}

	std::unordered_map<std::string, sf::Music*>::const_iterator music_it;
	for(music_it = m_Musics.begin(); music_it != m_Musics.end();) {
		if(music_it->second->getStatus() == sf::Music::Stopped) {
			delete (music_it->second);
			music_it = m_Musics.erase(music_it);
		} else {
			music_it++;
		}
	}
}