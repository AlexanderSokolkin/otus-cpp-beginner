#pragma once

#include "constants.h"
#include "field.h"
#include "indicator.h"
#include "button.h"

#include <mutex>


class Game
{
	enum GameState {
		Stopped,
		Running,
		Victory,
		Defeat
	};

public:
	Game(DifficultyLevel t_level = DifficultyLevel::Beginner);
	~Game() = default;


	int exec();

private:
	void setDifficultyLevel(DifficultyLevel t_level);
	GameElement determineGameElement(int t_x, int t_y);
	void startTimer();
	void stopTimer();


private:
	GameState			m_state;
	DifficultyLevel		m_level;


	float			m_windowWidth;
	float			m_windowHeight;
	unsigned		m_fieldCellsWidth;
	unsigned		m_fieldCellsHeight;
	unsigned		m_bombsCount;


	sf::Vector2f	m_fieldPosition;
	sf::Vector2f	m_bombsCounterPosition;
	sf::Vector2f	m_timerPosition;
	sf::Vector2f	m_buttonPosition;


	Field			m_field;
	Indicator		m_bombsCounter;
	Indicator		m_timer;
	Button			m_button;
	// TODO: menubar
	// TODO: config (global variable)


	std::mutex		m_timerMutex;
	std::thread*	m_timerThread;
};






