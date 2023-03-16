#pragma once

enum CellsOrder : unsigned {
	Zero = 0,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Close,
	Flag,
	Question,
	Bomb_White,
	Bomb_Red,
	Bomb_Cross
};

enum class CellState {
	Close,
	Open,
	Flagged,
	QuestionMark
};


enum Result {
	Victory,
	Defeat,
	Continue
};
