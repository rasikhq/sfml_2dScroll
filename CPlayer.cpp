#include "CApplication.h"
#include "CPlayer.h"

extern CApplication Game;

CPlayer::CPlayer(sf::Texture& playerTexture, sf::Texture& missileTexture)
{
	m_Player.setTexture(playerTexture);
	m_Missile.setTexture(missileTexture);
}

CPlayer::~CPlayer()
{
}

void CPlayer::update(float& dt) {
	std::vector<sf::Sprite>::iterator it;
	for(it = m_Shots.begin(); it != m_Shots.end();) {
		if(it->getPosition().x > Game.m_WindowResolution.width) {
			it = m_Shots.erase(it);
		} else {
			moveShot(dt, (*it));
			it++;
		}
	}
}

void CPlayer::shoot() {
	if(m_Shots.size() >= 3 || m_LastShoot.getElapsedTime().asSeconds() < 1)
		return;

	sf::Sprite missile = sf::Sprite(m_Missile);
	missile.setPosition(m_Player.getGlobalBounds().width + 5, m_Player.getPosition().y - 1);
	m_Shots.push_back(missile);

	m_LastShoot.restart();
}

void CPlayer::move(float& dt, signed short int direction) {
	float old_y = m_Velocity.y;
	m_Velocity.y = (direction * m_Units) * dt;

	// Create a "slow down" effect if direction is 0
	if(direction == 0) {
		m_Velocity.y = old_y > 0 ? old_y - dt*4 : old_y + dt*4;
	}

	if(m_Player.getPosition().y + m_Velocity.y >= Game.m_WindowResolution.height - m_Player.getGlobalBounds().height - 5
		|| m_Player.getPosition().y +  m_Velocity.y <= m_Player.getGlobalBounds().height + 5)
		return;

	m_Player.move(0, m_Velocity.y);
}

void CPlayer::moveShot(float& dt, sf::Sprite& missile) {
	missile.move(m_Units*dt, 0);
}

void CPlayer::draw() {
	Game.m_Window->draw(m_Player);
	std::vector<sf::Sprite>::iterator it;
	for(it = m_Shots.begin(); it != m_Shots.end(); it++) {
		Game.m_Window->draw(*it);
	}
}