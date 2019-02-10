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
public:
	static const short unsigned int MAX_ALLOWED_ROCKS = 5;
private:
	sf::Texture m_Meteor;
	sf::Vector2f m_Velocity;
	sf::Clock m_LastRock;

	std::vector<sf::Sprite> m_Rocks;

	float m_Units = -150.0f;
};