#pragma once
#include <SFML/Graphics.hpp>

class CScoreManager
{
public:
	CScoreManager();
	~CScoreManager();

	CScoreManager& operator+=(int pts) {
		m_Points += pts;
		if(m_Points > m_BestPoints)
			m_BestPoints = m_Points;
		this->update();
		return *this;
	}
	
	inline void setLevel(signed short int lvl) { m_Level = lvl; }
	inline signed short int getLevel() { return m_Level; }

	inline signed short int getPoints() { return m_Points; }

	void update();
	void reset();
	void draw();
private:
	sf::Text m_Score;
	sf::Text m_Best;

	signed short int m_Level = 1;
	signed short int m_Points;
	signed short int m_BestPoints;
};

