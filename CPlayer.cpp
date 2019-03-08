#include "CApplication.h"
#include "CPlayer.h"

extern CApplication Game;

CPlayer::CPlayer(sf::Texture& playerTexture, sf::Texture& missileTexture)
{
	m_Shots.reserve(MAX_ALLOWED_SHOTS);

	m_Player.setTexture(playerTexture);
	m_Missile.setTexture(missileTexture);

	m_Health.setFillColor(sf::Color::Green);
	m_Health.setPosition(Game.m_Window->getSize().x * 0.5f, Game.m_Window->getSize().y * 0.02f);

	m_HealthBackground.setFillColor(sf::Color::Color(50, 50, 50, 200));
	m_HealthBackground.setSize({200.0f + 2.0f, 15.0f + 2.0f});
	m_HealthBackground.setPosition(Game.m_Window->getSize().x * 0.5f - 1, Game.m_Window->getSize().y * 0.02f - 1);
}

CPlayer::~CPlayer()
{
}

void CPlayer::update(float& dt) {
	std::string itemCollide = Game.m_Items->collide(m_Player);
	if(itemCollide == "Rock") {
		m_fHealth -= Game.m_Items->ROCK_DAMAGE;
		if(m_fHealth <= 0) {
			onPlayerDie();
			return Game.Game_destroyPlayer();
		}
		// Set health color according to the percentage
		m_Health.setFillColor(sf::Color::Color(255*(1.0f - m_fHealth), 255*m_fHealth, 0));
	} else if(itemCollide == "Health") {
		m_fHealth += Game.m_Items->HEALTH_POINTS;
		if(m_fHealth > 1.0f)
			m_fHealth = 1.0f;
		// Set health color according to the percentage
		m_Health.setFillColor(sf::Color::Color(255 * (1.0f - m_fHealth), 255 * m_fHealth, 0));
	}

	std::vector<sf::Sprite>::iterator it;
	for(it = m_Shots.begin(); it != m_Shots.end();) {
		if(it->getPosition().x > Game.m_WindowResolution.width) {
			it = m_Shots.erase(it);
		} else if(Game.m_Items->collide(*it) == "Rock") {
			it = m_Shots.erase(it);
			onPlayerShootRock();
		} else {
			moveShot(dt, (*it));
			it++;
		}
	}
}

void CPlayer::shoot() {
	if(m_Shots.size() >= MAX_ALLOWED_SHOTS || m_LastShoot.getElapsedTime().asSeconds() < MIN_SHOT_WAIT)
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
	if(direction == 0 && dt < 1) {
		m_Velocity.y = old_y > 0 ? old_y - dt*(float)9.8 : old_y + dt*(float)9.8;
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
	Game.m_Window->draw(m_HealthBackground);
	m_Health.setSize({200.0f * m_fHealth, 15.0f});
	Game.m_Window->draw(m_Health);
	
	Game.m_Window->draw(m_Player);
	std::vector<sf::Sprite>::iterator it;
	for(it = m_Shots.begin(); it != m_Shots.end(); it++) {
		Game.m_Window->draw(*it);
	}
}

void CPlayer::onPlayerShootRock() {
	(*Game.m_ScoreManager) += 1;
	Game.m_SoundManager->playSound("hit.ogg");
}

void CPlayer::onPlayerDie() {
	Game.m_ScoreManager->reset();
}