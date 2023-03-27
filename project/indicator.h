#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Mouse.hpp"
#include "constants.h"



/*
* Данный класс реализует 7-ми сегментный индикатор.
* Для инициалищации необходимо передать путь до текстуры,
* размер одного сегмента и количество сегментов(необходимое нарисовать)
* Порядок сегментов в текстуре всегда один - "0123456789-"
*/



class Indicator : public sf::Drawable, public sf::Transformable
{
	enum Segment : unsigned {
		Zero = 0,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		Minus
	};

public:
	Indicator();
	void load(const std::string& t_segmentsSet,
			  const unsigned t_segmentSize[],
			  unsigned t_segmentsCount);

	bool setValue(int t_value);

	int value() const;
	std::string error() const;
	float width() const;
	float height() const;


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	sf::VertexArray		m_vertices;
	sf::Texture			m_texture;
	int					m_value;

	unsigned			m_segmentWidth;
	unsigned			m_segmentHeight;
	unsigned			m_segmentsCount;
	std::string			m_error;
};
