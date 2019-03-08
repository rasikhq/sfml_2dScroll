#pragma once
#include <SFML/Graphics.hpp>

class CItem;
class CItemsManager;

class CItem {
	friend class CItemsManager;
public:
	CItem(const std::string&, sf::Texture&);
	~CItem() {};

	inline sf::Sprite& get() { return element; }
private:
	sf::Texture m_Texture;
	sf::Vector2f m_Velocity;
	sf::Sprite element;

	std::string m_Type;

	float m_Units = -150.0f;

	bool b_Rotating = true;
};

class CItemsManager
{
	friend class CItem;
public:
	CItemsManager();
	~CItemsManager();

	std::string getRandomItem();
	void generate(std::string = "none");
	void update(float&);
	void move(float&, CItem&);
	void draw();
	void clear();
	std::string collide(sf::Sprite&, bool = true);
public:
	const double MIN_GENERATE_WAIT = 1.5f;
	static const short unsigned int MAX_ALLOWED_ITEMS = 5;
	
	const float ROCK_DAMAGE = 0.25f;
	const float HEALTH_POINTS = 0.25f;
private:
	sf::Clock m_LastItem;
	std::vector<CItem> m_Items;
};