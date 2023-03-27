#include "game.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <chrono>



Game::Game(DifficultyLevel t_level) :
	m_state(Stopped),
	m_windowWidth(0.f),
	m_windowHeight(0.f),
	m_fieldCellsWidth(0),
	m_fieldCellsHeight(0),
	m_bombsCount(0),
	m_fieldPosition(0.f, 0.f),
	m_bombsCounterPosition(0.f, 0.f),
	m_timerPosition(0.f, 0.f),
	m_buttonPosition(0.f, 0.f),
	m_timerThread(nullptr)
{
	// Если не удастся загрузить хотя бы одну текстуру,
	// то вылетит исключение runtime_error
	m_field.load(FIELD_TEXTURE_SET, FIELD_CELL_SIZE);
	m_bombsCounter.load(INDICATOR_TEXTURE_SET, INDICATOR_SEGMENT_SIZE, BOMBS_SEGMENTS_COUNT);
	m_timer.load(INDICATOR_TEXTURE_SET, INDICATOR_SEGMENT_SIZE, TIMER_SEGMENTS_COUNT);
	m_button.load(BUTTONS_TEXTURE_SET, BUTTON_SIZE,
				  SMILES_TEXTURE_SET, SMILE_SIZE);
	setDifficultyLevel(t_level);
}


int Game::exec()
{
	// Создаем окно
	sf::RenderWindow window(sf::VideoMode(m_windowWidth,
										  m_windowHeight),
							APP_NAME);

	// Ставим иконку
	auto image = sf::Image{};
	if (!image.loadFromFile(ICON_TEXTURE)) {
		// не удалось загрузить иконку окна, но приложение не останавливаем
	}
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	// Создаем игровой таймер
	sf::Clock clock;

	// Запускаем цикл обработки событий
	while(window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			// Если пользователь закрывает окно, то выходим из exec
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}

			// Если пользователь зажал ЛКМ, то подсвечиваем закрытые ячейки поля/кнопку
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Button::Left)
			{
				switch (determineGameElement(event.mouseButton.x, event.mouseButton.y)) {
					case GameElement::Field:
					{
						if (m_state == GameState::Victory || m_state == GameState::Defeat) {
							break;
						}
						FieldPressedResult res = m_field.onFieldPressed(sf::Vector2i{event.mouseButton.x - static_cast<int>(m_fieldPosition.x),
																					 event.mouseButton.y - static_cast<int>(m_fieldPosition.y)},
																		sf::Mouse::Button::Left);
						m_button.release();
						m_button.setSmile(res == FieldPressedResult::Lighting ? Smile_Press : Smile_Default);
						break;
					}
					case GameElement::Button:
						m_field.lightOffAllCells();
						m_button.press();
						m_button.setSmile(Smile_Default);
						break;
					default:
						m_field.lightOffAllCells();
						m_button.release();
						m_button.setSmile(Smile_Default);
						break;
				}
			}

			// Если пользователь водит курсором с нажатой ЛКМ, то подсвечиваем закрытые ячейки поля/кнопку
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
				event.type == sf::Event::MouseMoved)
			{
				switch (determineGameElement(event.mouseMove.x, event.mouseMove.y)) {
					case GameElement::Field:
					{
						if (m_state == GameState::Victory || m_state == GameState::Defeat) {
							break;
						}
						FieldPressedResult res = m_field.onFieldPressed(sf::Vector2i{event.mouseMove.x - static_cast<int>(m_fieldPosition.x),
																					 event.mouseMove.y - static_cast<int>(m_fieldPosition.y)},
																		sf::Mouse::Button::Left);
						m_button.release();
						m_button.setSmile(res == FieldPressedResult::Lighting ? Smile_Press : Smile_Default);
						break;
					}
					case GameElement::Button:
						m_field.lightOffAllCells();
						m_button.press();
						m_button.setSmile(Smile_Default);
						break;
					default:
						m_field.lightOffAllCells();
						m_button.release();
						switch (m_state) {
							case GameState::Victory:
								m_button.setSmile(Smile_Victory);
								break;
							case GameState::Defeat:
								m_button.setSmile(Smile_Defeat);
								break;
							default:
								m_button.setSmile(Smile_Default);
								break;
						}
						break;
				}
			}

			// Если пользователь отпустил ЛКМ, то открываем ячейку поля/рестарт игры
			if (event.type == sf::Event::MouseButtonReleased &&
				event.mouseButton.button == sf::Mouse::Button::Left)
			{
				switch (determineGameElement(event.mouseButton.x, event.mouseButton.y)) {
					case GameElement::Field:
					{
						if (m_state == GameState::Victory || m_state == GameState::Defeat) {
							break;
						}
						m_field.lightOffAllCells();
						FieldReleaseResult res = m_field.onFieldReleased(sf::Vector2i{event.mouseButton.x - static_cast<int>(m_fieldPosition.x),
																		  event.mouseButton.y - static_cast<int>(m_fieldPosition.y)});
						switch (res) {
							case FieldReleaseResult::Victory:
								m_state = GameState::Victory;
								m_button.setSmile(Smile_Victory);
								m_bombsCounter.setValue(0);
								stopTimer();
								break;
							case FieldReleaseResult::Defeat:
								m_state = GameState::Defeat;
								m_button.setSmile(Smile_Defeat);
								stopTimer();
								break;
							case FieldReleaseResult::Continue:
								if (m_state == GameState::Stopped) {
//									m_timerThread = new std::thread(&Game::startTimer, this);
									startTimer();
								}
								m_state = GameState::Running;
								m_button.setSmile(Smile_Default);
								break;
						}
						break;
					}
					case GameElement::Button:
						m_button.release();
						stopTimer();
						m_button.setSmile(Smile_Default);
						m_state = GameState::Stopped;
						m_bombsCounter.setValue(m_bombsCount);
						m_field.init(m_fieldCellsWidth, m_fieldCellsHeight, m_bombsCount);
						break;
					default:
						break;
				}
			}

			// Если пользователь нажимает на ПКМ, то ставим флажок на ячейку поля
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Button::Right)
			{
				if (m_state == GameState::Victory || m_state == GameState::Defeat) {
					break;
				}
				m_field.onFieldPressed(sf::Vector2i{event.mouseButton.x - static_cast<int>(m_fieldPosition.x),
													event.mouseButton.y - static_cast<int>(m_fieldPosition.y)},
													sf::Mouse::Button::Right);
				m_bombsCounter.setValue(static_cast<int>(m_field.bombs()) - m_field.flags());
			}
		}

		window.clear(sf::Color(189, 189, 189));
		window.draw(m_field);
		window.draw(m_bombsCounter);
		window.draw(m_timer);
		window.draw(m_button);
		window.display();
	}

	return 0;
}



void Game::setDifficultyLevel(DifficultyLevel t_level)
{
	switch (t_level) {
		case DifficultyLevel::Beginner:
			m_fieldCellsWidth = BEGINNER_FIELD_CELLS[0];
			m_fieldCellsHeight = BEGINNER_FIELD_CELLS[1];
			m_bombsCount = BEGINNER_BOMBS_COUNT;
			break;
		case DifficultyLevel::Amateur:
			m_fieldCellsWidth = AMATEUR_FIELD_CELLS[0];
			m_fieldCellsHeight = AMATEUR_FIELD_CELLS[1];
			m_bombsCount = AMATEUR_BOMBS_COUNT;
			break;
		case DifficultyLevel::Professional:
			m_fieldCellsWidth = PROFESSIONAL_FIELD_CELLS[0];
			m_fieldCellsHeight = PROFESSIONAL_FIELD_CELLS[1];
			m_bombsCount = PROFESSIONAL_BOMBS_COUNT;
			break;
	}
	m_windowWidth = m_fieldCellsWidth * FIELD_CELL_SIZE[0] +
					FIELD_AND_WINDOW_MARGIN * 2;
	m_windowHeight = static_cast<float>(MENU_HEIGHT) +
					 INDICATOR_AND_MENU_MARGIN + m_timer.height() +
					 FIELD_AND_INDICATORS_MARGIN +
					 m_fieldCellsHeight * FIELD_CELL_SIZE[1] +
					 FIELD_AND_WINDOW_MARGIN;

	m_field.init(m_fieldCellsWidth, m_fieldCellsHeight, m_bombsCount);
	m_bombsCounter.setValue(m_bombsCount);
	m_timer.setValue(0);
	m_button.setSmile(Smile_Default);

	// TODO : Заменить MENU_HEIGHT на m_menu.height()
	m_fieldPosition = sf::Vector2f{FIELD_AND_WINDOW_MARGIN,
								   static_cast<float>(MENU_HEIGHT) + INDICATOR_AND_MENU_MARGIN + m_timer.height() + FIELD_AND_INDICATORS_MARGIN};
	m_bombsCounterPosition = sf::Vector2f{INDICATOR_AND_SIDE_WINDOW_MARGIN,
										  static_cast<float>(MENU_HEIGHT) + INDICATOR_AND_MENU_MARGIN};
	m_timerPosition = sf::Vector2f{m_windowWidth - INDICATOR_AND_SIDE_WINDOW_MARGIN - m_timer.width(),
								   static_cast<float>(MENU_HEIGHT) + INDICATOR_AND_MENU_MARGIN};
	m_buttonPosition = sf::Vector2f{m_windowWidth / 2 - m_button.width() / 2,
									static_cast<float>(MENU_HEIGHT) + INDICATOR_AND_MENU_MARGIN + m_timer.height() / 2 - m_button.height() / 2};

	m_field.setPosition(m_fieldPosition);
	m_bombsCounter.setPosition(m_bombsCounterPosition);
	m_timer.setPosition(m_timerPosition);
	m_button.setPosition(m_buttonPosition);
}

GameElement Game::determineGameElement(int t_x, int t_y)
{
	// Детектим позиции кнопки и поля

	// Кнопка
	if (t_x >= m_buttonPosition.x && t_x <= m_buttonPosition.x + m_button.width() &&
		t_y >= m_buttonPosition.y && t_y <= m_buttonPosition.y + m_button.height())
	{
		return GameElement::Button;
	}

	// Поле
	if (t_x >= m_fieldPosition.x && t_x <= m_fieldPosition.x + m_field.width() &&
		t_y >= m_fieldPosition.y && t_y <= m_fieldPosition.y + m_field.height())
	{
		return GameElement::Field;
	}

	return GameElement::None;
}

void Game::startTimer()
{
	m_timerThread = new std::thread([&]{
		sf::Clock clock;
		while (true) {
			if (m_state != GameState::Running) {
				break;
			}

			m_timerMutex.lock();
			m_timer.setValue(clock.getElapsedTime().asSeconds());
			m_timerMutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});

}

void Game::stopTimer()
{
	if (!m_timerThread) {
		return;
	}

	m_timerThread->join();
	delete m_timerThread;
	m_timerThread = nullptr;
}
