#include "CApplication.h"
#include "CItemsManager.h"

extern CApplication Game;

/* CLASS: ITEM */

CItem::CItem(const std::string& type, sf::Texture& itemTexture)
	: m_Type(type)
	, m_Texture(itemTexture)
{
	//std::cout << "Created item! Type: " << m_Type << std::endl;
	element.setTexture(itemTexture);

	if(m_Type == "Health")
		b_Rotating = false;
}

/* CLASS: ITEM MANAGER */

CItemsManager::CItemsManager()
{
	m_Items.reserve(MAX_ALLOWED_ITEMS);
}

CItemsManager::~CItemsManager()
{
}

std::string CItemsManager::getRandomItem() {
	unsigned int i = rand() % 5 + 1;
	switch(i) {
		case 5:
			return "Health";
			break;
		default:
			return "Rock";
			break;
	}
}

void CItemsManager::generate(std::string type) {
	if(m_Items.size() >= MAX_ALLOWED_ITEMS || m_LastItem.getElapsedTime().asSeconds() < MIN_GENERATE_WAIT)
		return;

	if(type == "none")
		type = getRandomItem();
	
	CItem item(type, Game.m_Textures[type]);
	float range_y = Game.m_Window->getSize().y - item.get().getGlobalBounds().height;

	item.get().setPosition((float)Game.m_Window->getSize().x, (float)(rand() % (int)range_y) + (item.get().getGlobalBounds().height));
	item.get().setOrigin(item.get().getGlobalBounds().width / 2, item.get().getGlobalBounds().height / 2);
	if(type == "Rock")
		item.get().setRotation((float)(rand() % 180 + 0));
	m_Items.push_back(item);

	m_LastItem.restart();
}

void CItemsManager::update(float& dt) {
	generate();

	std::vector<CItem>::iterator it;
	for(it = m_Items.begin(); it != m_Items.end();) {
		if(it->get().getPosition().x + it->get().getGlobalBounds().width < 0.0f) {
			it = m_Items.erase(it);
		}
		else {
			move(dt, (*it));
			it++;
		}
	}
}

void CItemsManager::move(float& dt, CItem& item) {
	item.m_Velocity.x = item.m_Units * dt;
	
	if(item.b_Rotating) {
		float angle = dt * 180;
		item.get().rotate(angle);
	}
	item.get().move(item.m_Velocity.x, 0);
	//std::cout << "Moving some rock instance to X: " << (rock.getPosition().x + m_Velocity.x) << std::endl;
}

void CItemsManager::draw() {
	std::vector<CItem>::iterator it;
	for(it = m_Items.begin(); it != m_Items.end(); it++) {
		Game.m_Window->draw(it->get());
	}
}

void CItemsManager::clear() {
	m_Items.clear();
	m_LastItem.restart();
}

std::string CItemsManager::collide(sf::Sprite& element, bool destroy) {
	std::vector<CItem>::iterator it;
	for(it = m_Items.begin(); it != m_Items.end();) {
		if(it->get().getGlobalBounds().intersects(element.getGlobalBounds())) {
			std::string type = it->m_Type;
			if(destroy) {
				it = m_Items.erase(it);
			} else {
				it++;
			}
			return type;
		} else
			it++;
	}
	return "none";
}