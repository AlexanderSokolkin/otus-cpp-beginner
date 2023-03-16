#include "field.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <random>
#include <iostream>
#include <algorithm>



Field::Field() :
	sf::Drawable(), sf::Transformable(),
//	m_textureLoaded(false),
	m_fieldIsFill(false),
	m_cellSize(0),
	m_width(0),
	m_height(0),
	m_bombsCount(0),
	m_cellsCount(0),
	m_openCells(0)
{
	m_vertices.setPrimitiveType(sf::Quads);
}

bool Field::load(const std::string& t_cellsSet,
				 unsigned t_cellSize,
				 unsigned t_width,
				 unsigned t_height,
				 unsigned t_bombsCount)
{
	// load the cells texture
	if (!m_texture.loadFromFile(t_cellsSet)) {
		m_error = "Failed to upload texture file";
		return false;
	}
//	m_textureLoaded = true;

	// Clear old data
	m_vertices.clear();
	m_error.clear();
	m_bombs.clear();
	m_lightCells.clear();
	m_cells.clear();

	// init data and reserve memory
	m_fieldIsFill = false;
	m_openCells = 0;
	m_cellSize = t_cellSize;
	m_width = t_width;
	m_height = t_height;
	m_cellsCount = m_width * m_height;
	m_bombsCount = t_bombsCount;
	m_lightCells.reserve(8);
	m_cells.resize(t_width * t_height, Cell());
	m_vertices.resize(m_cellsCount * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned i = 0; i < m_width; ++i) {
		for (unsigned j = 0; j < m_height; ++j) {
			// get a pointer to the current cell's quad
			sf::Vertex* quad = &m_vertices[(i + j * m_width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * m_cellSize, j * m_cellSize);
			quad[1].position = sf::Vector2f((i + 1) * m_cellSize, j * m_cellSize);
			quad[2].position = sf::Vector2f((i + 1) * m_cellSize, (j + 1) * m_cellSize);
			quad[3].position = sf::Vector2f(i * m_cellSize, (j + 1) * m_cellSize);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(Close * m_cellSize, 0);
			quad[1].texCoords = sf::Vector2f((Close + 1) * m_cellSize, 0);
			quad[2].texCoords = sf::Vector2f((Close + 1) * m_cellSize, m_cellSize);
			quad[3].texCoords = sf::Vector2f(Close * m_cellSize, m_cellSize);
		}
	}

	return true;
}

void Field::onFieldPressed(sf::Vector2i t_pos)
{
	unsigned cell = t_pos.y / m_cellSize * m_width + t_pos.x / m_cellSize;

	if (m_cells[cell].state == CellState::Close) {
		if (m_lightCells.size() == 1 && m_lightCells.front() == cell) {
			return;
		}
		for (auto lightCell : m_lightCells) {
			setTile(lightCell, Close);
		}
		m_lightCells.clear();
		setTile(cell, Zero);
		m_lightCells.push_back(cell);
	}
}

Result Field::onFieldReleased(sf::Vector2i t_pos, sf::Mouse::Button t_button)
{
	unsigned cell = t_pos.y / m_cellSize * m_width + t_pos.x / m_cellSize;

	if (!m_fieldIsFill) {
		generateField(cell);
		m_fieldIsFill = true;
	}

	if (m_cells[cell].isBomb) {
		explosion(cell);
		return Defeat;
	}

	switch (m_cells[cell].state) {
		case CellState::Close:
			openCell(cell);
			break;
		case CellState::Open:
			if (openArea(cell) == Defeat) {
				return Defeat;
			}
			break;
		default:
			break;
	}

	if (m_openCells == m_cellsCount - m_bombsCount) {
		return Victory;
	}

	return Continue;
}

void Field::lightOffAllCells()
{
	for (auto lightCell : m_lightCells) {
		setTile(lightCell, Close);
	}
	m_lightCells.clear();
}



unsigned Field::flags() const
{
	unsigned res = 0;
	for (auto cell : m_cells) {
		if (cell.state == CellState::Flagged) {
			res++;
		}
	}
	return res;
}

unsigned Field::bombs() const
{
	return m_bombsCount;
}

std::string Field::error() const
{
	return m_error;
}



void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the cells texture
	states.texture = &m_texture;

	// draw the vertex array
	target.draw(m_vertices, states);
}



void Field::explosion(unsigned t_cell)
{
	for (int bomb : m_bombs) {
		setTile(bomb, (bomb == t_cell ? Bomb_Red : Bomb_White));
	}
}

void Field::openCell(unsigned t_cell)
{
	if (m_cells[t_cell].state == CellState::Close) {
		setTile(t_cell, static_cast<CellsOrder>(m_cells[t_cell].bombsAround));
		m_cells[t_cell].state = CellState::Open;

		if (!m_cells[t_cell].bombsAround) {
			std::vector<unsigned> cellsAround(findCellsAround(t_cell));
			for (auto cell : cellsAround) {
				openCell(cell);
			}
		}
		m_openCells++;
	}
}

Result Field::openArea(unsigned t_cell)
{
	std::vector<unsigned> cellsAround(findCellsAround(t_cell));
	int flagsAround = 0;
	for (unsigned cell : cellsAround) {
		if (m_cells[cell].state == CellState::Flagged) {
			flagsAround++;
		}
	}

	Result res = Continue;
	if (flagsAround == m_cells[t_cell].bombsAround) {
		for (unsigned cell : cellsAround) {
			if (m_cells[cell].state != CellState::Flagged) {
				if (!m_cells[cell].isBomb) {
					openCell(cell);
				} else {
					explosion(cell);
					res = Defeat;
				}
			}
		}
	}

	return res;
}



void Field::generateField(unsigned t_cell)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_cellsCount);

	while (m_bombs.size() < m_bombsCount) {
		unsigned bomb = dist(rng);
		if (bomb != t_cell) {
			m_bombs.insert(bomb);
		}
	}

	for (auto bomb : m_bombs) {
		m_cells[bomb].isBomb = true;
		std::vector<unsigned> cellsAround(findCellsAround(bomb));

		for (auto cell : cellsAround) {
			if (m_bombs.find(cell) == m_bombs.end()) {
				m_cells[cell].bombsAround++;
			}
		}
	}
}

void Field::setTile(unsigned t_cell, CellsOrder t_tile)
{
	sf::Vertex* quad = &m_vertices[t_cell * 4];
	quad[0].texCoords = sf::Vector2f(t_tile * m_cellSize, 0);
	quad[1].texCoords = sf::Vector2f((t_tile + 1) * m_cellSize, 0);
	quad[2].texCoords = sf::Vector2f((t_tile + 1) * m_cellSize, m_cellSize);
	quad[3].texCoords = sf::Vector2f(t_tile * m_cellSize, m_cellSize);
}

std::vector<unsigned> Field::findCellsAround(unsigned t_cell)
{
	std::vector<unsigned> cellsAround;
	cellsAround.resize(8);

	unsigned row = t_cell / m_width;
	unsigned col = t_cell % m_width;

	if (row != 0) {
		cellsAround.push_back(t_cell - m_width);
	}
	if (row != m_height - 1) {
		cellsAround.push_back(t_cell + m_width);
	}
	if (col != 0) {
		cellsAround.push_back(t_cell - 1);
	}
	if (col != m_width - 1) {
		cellsAround.push_back(t_cell + 1);
	}
	if (row != 0 && col != 0) {
		cellsAround.push_back(t_cell - m_width - 1);
	}
	if (row != 0 && col != m_width - 1) {
		cellsAround.push_back(t_cell - m_width + 1);
	}
	if (row != m_height - 1 && col != 0) {
		cellsAround.push_back(t_cell + m_width - 1);
	}
	if (row != m_height - 1 && col != m_width - 1) {
		cellsAround.push_back(t_cell + m_width + 1);
	}

	return cellsAround;
}
