#pragma once
#include <SFML/Graphics.hpp>

class CRocksManager
{
public:
	CRocksManager(sf::Texture&);
	~CRocksManager();

	void generate();
	void update(float&);
	void move(float&, sf::Sprite&);
	void draw();
	void clear();
	bool collide(sf::Sprite&, bool = true);
public:
	static const short unsigned int MAX_ALLOWED_ROCKS = 5;
	const double MIN_GENERATE_WAIT = 1.5f;
private:
	sf::Texture m_Meteor;
	sf::Vector2f m_Velocity;
	sf::Clock m_LastRock;

	std::vector<sf::Sprite> m_Rocks;

	float m_Units = -150.0f;
};