#include "indicator.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <stdexcept>
#include <cmath>


Indicator::Indicator() :
	sf::Drawable(), sf::Transformable(),
	m_value(0),
	m_segmentWidth(0),
	m_segmentHeight(0),
	m_segmentsCount(0)
{
	m_vertices.setPrimitiveType(sf::Quads);
}

void Indicator::load(const std::string& t_segmentsSet,
					 const unsigned t_segmentSize[],
					 unsigned t_segmentsCount)
{
	// load the cells texture
	if (!m_texture.loadFromFile(t_segmentsSet)) {
		throw std::runtime_error("Failed to upload indicator texture file");
	}

	m_segmentWidth = t_segmentSize[0];
	m_segmentHeight = t_segmentSize[1];

	m_segmentsCount = t_segmentsCount;

	m_vertices.resize(m_segmentsCount * 4);

	for (unsigned i = 0; i < m_segmentsCount; ++i) {
		sf::Vertex* quad = &m_vertices[i * 4];

		// define its 4 corners
		quad[0].position = sf::Vector2f(i * m_segmentWidth, 0);
		quad[1].position = sf::Vector2f((i + 1) * m_segmentWidth, 0);
		quad[2].position = sf::Vector2f((i + 1) * m_segmentWidth, m_segmentHeight);
		quad[3].position = sf::Vector2f(i * m_segmentWidth, m_segmentHeight);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(Segment::Zero * m_segmentWidth, 0);
		quad[1].texCoords = sf::Vector2f((Segment::Zero + 1) * m_segmentWidth, 0);
		quad[2].texCoords = sf::Vector2f((Segment::Zero + 1) * m_segmentWidth, m_segmentHeight);
		quad[3].texCoords = sf::Vector2f(Segment::Zero * m_segmentWidth, m_segmentHeight);
	}
}

bool Indicator::setValue(int t_value)
{
	if (m_value == t_value) {
		return true;
	}

	bool minus = t_value < 0;
	unsigned absVal = static_cast<unsigned>(std::abs(t_value));
	unsigned digitsCount = t_value == 0 ? 1 : std::log10(absVal) + 1;

	if ((minus && digitsCount > m_segmentsCount - 1) ||
		(digitsCount > m_segmentsCount))
	{
		m_error = "Not enough segments";
		return false;
	}

	m_value = t_value;
	for (int i = static_cast<int>(m_segmentsCount) - 1; i >= 0; --i) {
		unsigned digit = absVal % 10;
		absVal /= 10;
		if (i == 0 && minus) {
			digit = static_cast<unsigned>(Segment::Minus);
		}

		sf::Vertex* quad = &m_vertices[i * 4];
		quad[0].texCoords = sf::Vector2f(digit * m_segmentWidth, 0);
		quad[1].texCoords = sf::Vector2f((digit + 1) * m_segmentWidth, 0);
		quad[2].texCoords = sf::Vector2f((digit + 1) * m_segmentWidth, m_segmentHeight);
		quad[3].texCoords = sf::Vector2f(digit * m_segmentWidth, m_segmentHeight);
	}

	return true;
}

int Indicator::value() const
{
	return m_value;
}

std::string Indicator::error() const
{
	return m_error;
}

float Indicator::width() const
{
	return static_cast<float>(m_segmentWidth * m_segmentsCount);
}

float Indicator::height() const
{
	return static_cast<float>(m_segmentHeight);
}



void Indicator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_vertices, states);
}
