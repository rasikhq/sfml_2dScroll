#include "CApplication.h"
#include "CScoreManager.h"

extern CApplication Game;

CScoreManager::CScoreManager()
	: m_Points(0)
	, m_BestPoints(0)
{
	m_Score.setFont((*Game.m_Font));
	m_Score.setCharacterSize(25);
	m_Score.setPosition(5, 5);

	m_Best.setFont((*Game.m_Font));
	m_Best.setCharacterSize(25);
	this->update();
}

CScoreManager::~CScoreManager()
{
}

void CScoreManager::update() {
	m_Score.setString("Score: " + std::to_string(m_Points));
	m_Best.setString("Best Score: " + std::to_string(m_BestPoints));
	m_Best.setPosition(m_Score.getGlobalBounds().width + m_Score.getCharacterSize(), 5);

	if(m_Points == 50) {
		Game.Game_createEnvironment("level_2");
	}
}

void CScoreManager::reset() {
	m_Points = 0;
	this->update();
}

void CScoreManager::draw() {
	Game.m_Window->draw(m_Score);
	Game.m_Window->draw(m_Best);
}