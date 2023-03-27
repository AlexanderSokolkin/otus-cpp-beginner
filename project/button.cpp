#include "button.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <stdexcept>



Button::Button() :
	sf::Drawable(), sf::Transformable(),
	m_buttonWidth(0),
	m_buttonHeight(0),
	m_iconWidth(0),
	m_iconHeight(0),
	m_buttonState(Button_Unpressed),
	m_smilesState(Smile_Default)
{

}

void Button::load(const std::string& t_buttonsSet,
				  const unsigned t_buttonSize[],
				  const std::string& t_iconSet,
				  const unsigned t_iconSize[])
{
	if (!m_buttonTexture.loadFromFile(t_buttonsSet)) {
		throw std::runtime_error("Failed to upload button texture file");
	}
	if (!m_iconTexture.loadFromFile(t_iconSet)) {
		throw std::runtime_error("Failed to upload icon texture file");
	}


	m_buttonWidth = static_cast<int>(t_buttonSize[0]);
	m_buttonHeight = static_cast<int>(t_buttonSize[1]);
	m_iconWidth = static_cast<int>(t_iconSize[0]);
	m_iconHeight = static_cast<int>(t_iconSize[1]);
	m_button.setSize(sf::Vector2f{static_cast<float>(t_buttonSize[0]),
								  static_cast<float>(t_buttonSize[1])});
	m_icon.setSize(sf::Vector2f{static_cast<float>(t_iconSize[0]),
								static_cast<float>(t_iconSize[1])});

	m_button.setTexture(&m_buttonTexture);
	m_button.setTextureRect(sf::IntRect{0, 0, m_buttonWidth, m_buttonHeight});

	m_icon.setTexture(&m_iconTexture);
	m_icon.setTextureRect(sf::IntRect{0, 0, m_iconWidth, m_iconHeight});
}

void Button::press()
{
	if (m_buttonState == Button_Pressed) {
		return;
	}

	m_button.setTextureRect(sf::IntRect{static_cast<int>(Button_Pressed * m_buttonWidth),
										0,
										m_buttonWidth,
										m_buttonHeight});
	m_buttonState = Button_Pressed;
}

void Button::release()
{
	if (m_buttonState == Button_Unpressed) {
		return;
	}

	m_button.setTextureRect(sf::IntRect{static_cast<int>(Button_Unpressed * m_buttonWidth),
										0,
										m_buttonWidth,
										m_buttonHeight});
	m_buttonState = Button_Unpressed;
}

void Button::setSmile(SmilesOrder t_smile)
{
	if (m_smilesState == t_smile) {
		return;
	}

	m_icon.setTextureRect(sf::IntRect{static_cast<int>(t_smile * m_iconWidth),
									  0,
									  m_iconWidth,
									  m_iconHeight});
	m_smilesState = t_smile;
}

std::string Button::error() const
{
	return m_error;
}

float Button::width() const
{
	return static_cast<float>(m_buttonWidth);
}

float Button::height() const
{
	return static_cast<float>(m_buttonHeight);
}



void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_button, states);
	sf::Transform iconTransform;
	sf::Vector2f offsetIcon{static_cast<float>((m_buttonWidth - m_iconWidth) / 2),
							static_cast<float>((m_buttonHeight - m_iconHeight) / 2)};
	iconTransform.translate(offsetIcon);
	states.transform *= iconTransform;
	target.draw(m_icon, states);
}
