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
	bool load(const std::string& t_cellsSet,
			  unsigned t_cellSize,
			  unsigned t_width,
			  unsigned t_height,
			  unsigned t_bombsCount);


	void onFieldPressed(sf::Vector2i t_pos);
	Result onFieldReleased(sf::Vector2i t_pos, sf::Mouse::Button t_button);
	void lightOffAllCells();


	unsigned flags() const;
	unsigned bombs() const;
	std::string error() const;


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	void openCell(unsigned t_cell);
	Result openArea(unsigned t_cell);
	void explosion(unsigned t_cell);

	void generateField(unsigned t_cell);
	void setTile(unsigned t_cell, CellsOrder t_tile);
	std::vector<unsigned> findCellsAround(unsigned t_cell);


private:
	struct Cell {
		unsigned bombsAround = 0;
		bool isBomb = false;
		CellState state = CellState::Close;
	};

	sf::VertexArray			m_vertices;
	sf::Texture				m_texture;
//	bool					m_textureLoaded;
	bool					m_fieldIsFill;
	std::string				m_error;
	unsigned				m_cellSize;
	unsigned				m_width;
	unsigned				m_height;
	unsigned				m_bombsCount;
	unsigned				m_cellsCount;
	std::set<unsigned>		m_bombs;
	std::vector<unsigned>	m_lightCells;
	std::vector<Cell>		m_cells;
	unsigned				m_openCells;
};
