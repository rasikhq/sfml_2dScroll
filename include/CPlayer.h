#pragma once
#include <SFML/Graphics.hpp>

class CPlayer
{
public:
	CPlayer(sf::Texture&, sf::Texture&);
	~CPlayer();

	sf::Sprite& operator()(void) {
		return m_Player;
	}

	void update(float&);
	void move(float&, signed short int = 0);
	void moveShot(float&, sf::Sprite&);
	void shoot();
	void draw();
private:
	sf::Sprite m_Player;
	sf::Sprite m_Missile;
	sf::Vector2f m_Velocity;
	sf::Clock m_LastShoot;

	std::vector<sf::Sprite> m_Shots;

	float m_Units = 150.0f;
};