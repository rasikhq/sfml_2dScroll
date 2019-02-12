#include "CApplication.h"
#include "CRocksManager.h"

extern CApplication Game;

CRocksManager::CRocksManager(sf::Texture& meteorTexture)
	: m_Meteor(meteorTexture)
{
	m_Rocks.reserve(MAX_ALLOWED_ROCKS);
}

CRocksManager::~CRocksManager()
{
}

void CRocksManager::generate() {
	if(m_Rocks.size() >= MAX_ALLOWED_ROCKS || m_LastRock.getElapsedTime().asSeconds() < MIN_GENERATE_WAIT)
		return;
	
	sf::Sprite rock = sf::Sprite(m_Meteor);
	float range_y = Game.m_Window->getSize().y - rock.getGlobalBounds().height;

	rock.setPosition((float)Game.m_Window->getSize().x, (float)(rand() % (int)range_y) + (rock.getGlobalBounds().height));
	rock.setOrigin(rock.getGlobalBounds().width / 2, rock.getGlobalBounds().height / 2);
	rock.setRotation((float)(rand() % 180 + 0));
	m_Rocks.push_back(rock);

	m_LastRock.restart();
}

void CRocksManager::update(float& dt) {
	generate();

	std::vector<sf::Sprite>::iterator it;
	for(it = m_Rocks.begin(); it != m_Rocks.end();) {
		if(it->getPosition().x + it->getGlobalBounds().width < 0.0f) {
			it = m_Rocks.erase(it);
		}
		else {
			move(dt, (*it));
			it++;
		}
	}
}

void CRocksManager::move(float& dt, sf::Sprite& rock) {
	m_Velocity.x = m_Units * dt;
	
	float angle = dt * 180;

	rock.rotate(angle);
	rock.move(m_Velocity.x, 0);
	//std::cout << "Moving some rock instance to X: " << (rock.getPosition().x + m_Velocity.x) << std::endl;
}

void CRocksManager::draw() {
	std::vector<sf::Sprite>::iterator it;
	for(it = m_Rocks.begin(); it != m_Rocks.end(); it++) {
		Game.m_Window->draw(*it);
	}
}

void CRocksManager::clear() {
	m_Rocks.clear();
	m_LastRock.restart();
}

bool CRocksManager::collide(sf::Sprite& element, bool destroy) {
	std::vector<sf::Sprite>::iterator it;
	for(it = m_Rocks.begin(); it != m_Rocks.end();) {
		if(it->getGlobalBounds().intersects(element.getGlobalBounds())) {
			if(destroy) {
				it = m_Rocks.erase(it);
			} else {
				it++;
			}
			return true;
		} else
			it++;
	}
	return false;
}