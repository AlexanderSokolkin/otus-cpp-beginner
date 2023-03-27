#pragma once


// common
extern const char*		APP_NAME;
extern const unsigned	TIMER_SEGMENTS_COUNT;
extern const unsigned	BOMBS_SEGMENTS_COUNT;

// 	Beginner
extern const unsigned	BEGINNER_BOMBS_COUNT;
extern const unsigned	BEGINNER_FIELD_CELLS[2];

// Amateur
extern const unsigned	AMATEUR_BOMBS_COUNT;
extern const unsigned	AMATEUR_FIELD_CELLS[2];

// Professional
extern const unsigned	PROFESSIONAL_BOMBS_COUNT;
extern const unsigned	PROFESSIONAL_FIELD_CELLS[2];




// margins
extern const float		FIELD_AND_WINDOW_MARGIN;
extern const float		FIELD_AND_INDICATORS_MARGIN;
extern const float		INDICATOR_AND_MENU_MARGIN;
extern const float		INDICATOR_AND_SIDE_WINDOW_MARGIN;



// textures
extern const char*		FIELD_TEXTURE_SET;
extern const unsigned	FIELD_CELL_SIZE[2];

extern const char*		INDICATOR_TEXTURE_SET;
extern const unsigned	INDICATOR_SEGMENT_SIZE[2];

extern const char*		BUTTONS_TEXTURE_SET;
extern const unsigned	BUTTON_SIZE[2];

extern const char*		SMILES_TEXTURE_SET;
extern const unsigned	SMILE_SIZE[2];

extern const char*		ICON_TEXTURE;
extern const unsigned	MENU_HEIGHT;





enum CellsOrder : unsigned {
	Cell_Zero = 0,
	Cell_One,
	Cell_Two,
	Cell_Three,
	Cell_Four,
	Cell_Five,
	Cell_Six,
	Cell_Seven,
	Cell_Eight,
	Cell_Close,
	Cell_Flag,
	Cell_Question,
	Cell_Bomb_White,
	Cell_Bomb_Red,
	Cell_Bomb_Cross
};

enum ButtonsOrder : unsigned {
	Button_Unpressed = 0,
	Button_Pressed
};

enum SmilesOrder : unsigned {
	Smile_Default = 0,
	Smile_Press,
	Smile_Defeat,
	Smile_Victory
};

enum SegmentsOrder : unsigned {
	Segment_Zero = 0,
	Segment_One,
	Segment_Two,
	Segment_Three,
	Segment_Four,
	Segment_Five,
	Segment_Six,
	Segment_Seven,
	Segment_Eight,
	Segment_Nine,
	Segment_Minus
};




enum class CellState {
	Close,
	Open,
	Flagged,
	QuestionMark
};

enum class FieldPressedResult {
	Lighting,
	None
};

enum class FieldReleaseResult {
	Victory,
	Defeat,
	Continue
};

enum class DifficultyLevel {
	Beginner,
	Amateur,
	Professional
};

enum class GameElement {
	// TODO : menubar
	Timer,
	Button,
	BombsCounter,
	Field,
	None
};
