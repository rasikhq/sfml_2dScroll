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
	
	void update();
	void reset();
	void draw();
private:
	sf::Text m_Score;
	sf::Text m_Best;

	signed short int m_Points;
	signed short int m_BestPoints;
};

