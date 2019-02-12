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

	void onPlayerShootRock();
	void onPlayerDie();
public:
	static const short unsigned int MAX_ALLOWED_SHOTS = 3;
	const double MIN_SHOT_WAIT = 0.5f;
private:
	sf::Sprite m_Player;
	sf::Sprite m_Missile;
	sf::Vector2f m_Velocity;
	sf::Clock m_LastShoot;

	std::vector<sf::Sprite> m_Shots;

	float m_Units = 150.0f;
};