#include "field.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdexcept>



Field::Field() :
	sf::Drawable(), sf::Transformable(),
	m_fieldIsFill(false),
	m_cellWidth(0),
	m_cellHeight(0),
	m_widthCellsCount(0),
	m_heightCellsCount(0),
	m_cellsCount(0),
	m_bombsCount(0),
	m_openCells(0)
{
	m_vertices.setPrimitiveType(sf::Quads);
}

void Field::load(const char* t_cellsSet,
				 const unsigned t_cellSize[])
{
	if (!m_texture.loadFromFile(t_cellsSet)) {
		throw std::runtime_error("Failed to upload field texture file");
	}
	m_cellWidth = t_cellSize[0];
	m_cellHeight = t_cellSize[1];
}

void Field::init(unsigned t_widthCellsCount,
				 unsigned t_heightCellsCount,
				 unsigned t_bombsCount)
{
	// Clear old data
	m_vertices.clear();
	m_error.clear();
	m_bombs.clear();
	m_lightCells.clear();
	m_cells.clear();

	// default data and reserve memory
	m_fieldIsFill = false;
	m_openCells = 0;
	m_widthCellsCount = t_widthCellsCount;
	m_heightCellsCount = t_heightCellsCount;
	m_cellsCount = m_widthCellsCount * m_heightCellsCount;
	m_bombsCount = t_bombsCount;
	m_lightCells.reserve(8);
	m_cells.resize(m_widthCellsCount * m_heightCellsCount, Cell{});
	m_vertices.resize(m_cellsCount * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned i = 0; i < m_widthCellsCount; ++i) {
		for (unsigned j = 0; j < m_heightCellsCount; ++j) {
			// get a pointer to the current cell's quad
			sf::Vertex* quad = &m_vertices[(i + j * m_widthCellsCount) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * m_cellWidth,			j * m_cellHeight);
			quad[1].position = sf::Vector2f((i + 1) * m_cellWidth,		j * m_cellHeight);
			quad[2].position = sf::Vector2f((i + 1) * m_cellWidth,		(j + 1) * m_cellHeight);
			quad[3].position = sf::Vector2f(i * m_cellWidth,			(j + 1) * m_cellHeight);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(Cell_Close * m_cellWidth,		0);
			quad[1].texCoords = sf::Vector2f((Cell_Close + 1) * m_cellWidth,	0);
			quad[2].texCoords = sf::Vector2f((Cell_Close + 1) * m_cellWidth,	m_cellHeight);
			quad[3].texCoords = sf::Vector2f(Cell_Close * m_cellWidth,		m_cellHeight);
		}
	}
}

FieldPressedResult Field::onFieldPressed(sf::Vector2i t_pos, sf::Mouse::Button t_button)
{
	unsigned cell = (t_pos.y - 1) / static_cast<int>(m_cellHeight) * m_widthCellsCount + (t_pos.x - 1) / static_cast<int>(m_cellWidth);

	switch (t_button) {
		case sf::Mouse::Button::Right:
			toggledCell(cell);
			break;
		case sf::Mouse::Button::Left:
			if (m_cells[cell].state == CellState::Close) {
				if (m_lightCells.size() == 1 && m_lightCells.front() == cell) {
					return FieldPressedResult::Lighting;
				}
				for (auto lightCell : m_lightCells) {
					setTile(lightCell, Cell_Close);
				}
				m_lightCells.clear();
				setTile(cell, Cell_Zero);
				m_lightCells.push_back(cell);
				return FieldPressedResult::Lighting;
			}
			else if (m_cells[cell].state == CellState::Open) {
				lightOffAllCells();
				for (auto nearCell : findCellsAround(cell)) {
					if (m_cells[nearCell].state == CellState::Close) {
						setTile(nearCell, Cell_Zero);
						m_lightCells.push_back(nearCell);
					}
				}
				if (!m_lightCells.empty()) {
					return FieldPressedResult::Lighting;
				}
			}
			else {
				lightOffAllCells();
			}
			break;
		default:
			break;
	}

	return FieldPressedResult::None;
}

FieldReleaseResult Field::onFieldReleased(sf::Vector2i t_pos)
{
	unsigned cell = (t_pos.y - 1) / static_cast<int>(m_cellHeight) * m_widthCellsCount + (t_pos.x - 1) / static_cast<int>(m_cellWidth);

	if (m_cells[cell].state == CellState::Flagged ||
		m_cells[cell].state == CellState::QuestionMark)
	{
		return FieldReleaseResult::Continue;
	}

	if (!m_fieldIsFill) {
		generateField(cell);
		m_fieldIsFill = true;
	}

	if (m_cells[cell].isBomb) {
		explosion(cell);
		return FieldReleaseResult::Defeat;
	}

	switch (m_cells[cell].state) {
		case CellState::Close:
			openCell(cell);
			break;
		case CellState::Open:
			if (openArea(cell) == FieldReleaseResult::Defeat) {
				return FieldReleaseResult::Defeat;
			}
			break;
		default:
			break;
	}

	if (m_openCells == m_cellsCount - m_bombsCount) {
		for (auto bomb : m_bombs) {
			m_cells[bomb].state = CellState::Flagged;
			setTile(bomb, Cell_Flag);
		}
		return FieldReleaseResult::Victory;
	}

	return FieldReleaseResult::Continue;
}

void Field::lightOffAllCells()
{
	for (auto lightCell : m_lightCells) {
		setTile(lightCell, Cell_Close);
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

float Field::width() const
{
	return static_cast<float>(m_widthCellsCount * m_cellWidth);
}

float Field::height() const
{
	return static_cast<float>(m_heightCellsCount * m_cellHeight);
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
	for (unsigned i = 0; i < m_cells.size(); ++i) {
		if (m_cells[i].isBomb) {
			if (i == t_cell) {
				setTile(i, Cell_Bomb_Red);
				continue;
			}
			if (m_cells[i].state == CellState::Close) {
				setTile(i, Cell_Bomb_White);
			}
		}
		else {
			if (m_cells[i].state == CellState::Flagged) {
				setTile(i, Cell_Bomb_Cross);
			}
		}
	}
}

void Field::toggledCell(unsigned t_cell)
{
	switch (m_cells[t_cell].state) {
		case CellState::Open:
			break;
		case CellState::Close:
			m_cells[t_cell].state = CellState::Flagged;
			setTile(t_cell, Cell_Flag);
			break;
		case CellState::Flagged:
			m_cells[t_cell].state = CellState::QuestionMark;
			setTile(t_cell, Cell_Question);
			break;
		case CellState::QuestionMark:
			m_cells[t_cell].state = CellState::Close;
			setTile(t_cell, Cell_Close);
			break;
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

FieldReleaseResult Field::openArea(unsigned t_cell)
{
	std::vector<unsigned> cellsAround(findCellsAround(t_cell));
	int flagsAround = 0;
	for (unsigned cell : cellsAround) {
		if (m_cells[cell].state == CellState::Flagged) {
			flagsAround++;
		}
	}

	FieldReleaseResult res = FieldReleaseResult::Continue;
	if (flagsAround == m_cells[t_cell].bombsAround) {
		for (unsigned cell : cellsAround) {
			if (m_cells[cell].state != CellState::Flagged) {
				if (!m_cells[cell].isBomb) {
					openCell(cell);
				} else {
					explosion(cell);
					res = FieldReleaseResult::Defeat;
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
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_cellsCount - 1);

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
	quad[0].texCoords = sf::Vector2f(t_tile * m_cellWidth,			0);
	quad[1].texCoords = sf::Vector2f((t_tile + 1) * m_cellWidth,	0);
	quad[2].texCoords = sf::Vector2f((t_tile + 1) * m_cellWidth,	m_cellHeight);
	quad[3].texCoords = sf::Vector2f(t_tile * m_cellWidth,			m_cellHeight);
}

std::vector<unsigned> Field::findCellsAround(unsigned t_cell)
{
	std::vector<unsigned> cellsAround;
	cellsAround.reserve(8);

	unsigned row = t_cell / m_widthCellsCount;
	unsigned col = t_cell % m_widthCellsCount;

	if (row != 0) {
		cellsAround.push_back(t_cell - m_widthCellsCount);
	}
	if (row != m_heightCellsCount - 1) {
		cellsAround.push_back(t_cell + m_widthCellsCount);
	}
	if (col != 0) {
		cellsAround.push_back(t_cell - 1);
	}
	if (col != m_widthCellsCount - 1) {
		cellsAround.push_back(t_cell + 1);
	}
	if (row != 0 && col != 0) {
		cellsAround.push_back(t_cell - m_widthCellsCount - 1);
	}
	if (row != 0 && col != m_widthCellsCount - 1) {
		cellsAround.push_back(t_cell - m_widthCellsCount + 1);
	}
	if (row != m_heightCellsCount - 1 && col != 0) {
		cellsAround.push_back(t_cell + m_widthCellsCount - 1);
	}
	if (row != m_heightCellsCount - 1 && col != m_widthCellsCount - 1) {
		cellsAround.push_back(t_cell + m_widthCellsCount + 1);
	}

	return cellsAround;
}
