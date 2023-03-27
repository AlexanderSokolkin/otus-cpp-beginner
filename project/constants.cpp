#include "constants.h"


// common
const char*		APP_NAME = "Saper";
const unsigned	TIMER_SEGMENTS_COUNT = 3;
const unsigned	BOMBS_SEGMENTS_COUNT = 3;




// Beginner
const unsigned	BEGINNER_BOMBS_COUNT = 10;
const unsigned	BEGINNER_FIELD_CELLS[2] = {10, 10};

// Amateur
const unsigned	AMATEUR_BOMBS_COUNT = 40;
const unsigned	AMATEUR_FIELD_CELLS[2] = {15, 15};

// Professional
const unsigned	PROFESSIONAL_BOMBS_COUNT = 100;
const unsigned	PROFESSIONAL_FIELD_CELLS[2] = {30, 15};




// margins
const float		FIELD_AND_WINDOW_MARGIN				=	25.f;
const float		FIELD_AND_INDICATORS_MARGIN			=	25.f;
const float		INDICATOR_AND_MENU_MARGIN			=	25.f;
const float		INDICATOR_AND_SIDE_WINDOW_MARGIN	=	35.f;




// textures
const char*		FIELD_TEXTURE_SET			=	"textures/cells.png";
const unsigned	FIELD_CELL_SIZE[2]			=	{25, 25};


const char*		INDICATOR_TEXTURE_SET		=	"textures/segments.png";
const unsigned	INDICATOR_SEGMENT_SIZE[2]	=	{25, 35};


const char*		BUTTONS_TEXTURE_SET			=	"textures/button.png";
const unsigned	BUTTON_SIZE[2]				=	{35, 35};


const char*		SMILES_TEXTURE_SET			=	"textures/smiles.png";
const unsigned	SMILE_SIZE[2]				=	{25, 25};


const char*		ICON_TEXTURE				=	"textures/icon.png";
const unsigned	MENU_HEIGHT					=	25;
