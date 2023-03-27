#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "constants.h"



class Button : public sf::Drawable, public sf::Transformable
{
	enum State {
		Unpressed = 0,
		Pressed
	};

public:
	Button();
	void load(const std::string& t_buttonsSet,
			  const unsigned t_buttonSize[],
			  const std::string& t_iconSet,
			  const unsigned t_iconSize[]);

	void press();
	void release();

	void setSmile(SmilesOrder t_smile);

	std::string error() const;
	float width() const;
	float height() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RectangleShape	m_button;
	sf::RectangleShape	m_icon;
	sf::Texture			m_buttonTexture;
	sf::Texture			m_iconTexture;

	int					m_buttonWidth;
	int					m_buttonHeight;
	int					m_iconWidth;
	int					m_iconHeight;
	ButtonsOrder		m_buttonState;
	SmilesOrder			m_smilesState;
	std::string			m_error;
};
