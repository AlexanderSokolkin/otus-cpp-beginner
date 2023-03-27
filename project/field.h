#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Mouse.hpp"
#include "constants.h"

#include <vector>
#include <set>
#include <string>



class Field : public sf::Drawable, public sf::Transformable
{
public:
	Field();
	void load(const char* t_cellsSet,
			  const unsigned t_cellSize[]);
	void init(unsigned t_width,
			  unsigned t_height,
			  unsigned t_bombsCount);


	FieldPressedResult onFieldPressed(sf::Vector2i t_pos, sf::Mouse::Button t_button);
	FieldReleaseResult onFieldReleased(sf::Vector2i t_pos);
	void lightOffAllCells();


	unsigned flags() const;
	unsigned bombs() const;
	std::string error() const;
	float width() const;
	float height() const;


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	void toggledCell(unsigned t_cell);
	void openCell(unsigned t_cell);
	FieldReleaseResult openArea(unsigned t_cell);
	void explosion(unsigned t_cell);

	void generateField(unsigned t_cell);
	void setTile(unsigned t_cell, CellsOrder t_tile);
	std::vector<unsigned> findCellsAround(unsigned t_cell);


private:
	struct Cell {
		Cell() : bombsAround(0), isBomb(false), state(CellState::Close) {}
		unsigned bombsAround;
		bool isBomb;
		CellState state;
	};

	sf::VertexArray			m_vertices;
	sf::Texture				m_texture;

	std::string				m_error;
	bool					m_fieldIsFill;
	unsigned				m_cellWidth;
	unsigned				m_cellHeight;
	unsigned				m_widthCellsCount;
	unsigned				m_heightCellsCount;
	unsigned				m_cellsCount;
	unsigned				m_bombsCount;
	unsigned				m_openCells;

	std::set<unsigned>		m_bombs;
	std::vector<unsigned>	m_lightCells;
	std::vector<Cell>		m_cells;
};
