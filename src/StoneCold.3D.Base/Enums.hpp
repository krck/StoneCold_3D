
#ifndef STONECOLD_ENUMS_H
#define STONECOLD_ENUMS_H

namespace StoneCold::Base {

	//
	// All possible Camera movements
	//
	enum class MoveDirection { 
		LEFT, 
		RIGHT, 
		UP,
		DOWN,
		// BELOW ARE ONLY USED TO DEBUG
		FORWARD,
		BACKWARD
	};

	//
	// All the available Level Types
	// (Maybe replace with a runtime-check of which Map-Textures are available)
	//
	enum class LevelType {
		Grassland,
		Desert,
		Castle,
		Highlands,
		Arctic
	};

	//
	// ResourceLifeTimes
	// - Intro, Game and Menu: All have global/infinite lifetime (as long as the game is running) like Player Character, Fonts, GUI Elements
	//						   but are split, so they could potentially be unloaded, cleaned up, refreshed, etc. separatley
	// - Level:	These Resources are only needed during a specific Level (Ground-Textures, NPC Animations, ...) and will be loaded
	//          and unloaded frequently. Only the Resources for one Level will be held in memory at any given time
	//
	enum class ResourceLifeTime {
		Intro,
		Game,
		Menu,
		Level
	};

	//
	// Event Code send with a custom SDL_UserEvent
	// (Not stronly typed, because SDL_Event Codes are int so it must be cast) 
	//
	enum EventCode {
		Ping,
		ChangeLevel,
		Quit
	};

	//
	// AI Decision Flags (32-bit masks)
	// AiDecision::PLAYER_VISIBLE	00000000000000000000000000000001
	// AiDecision::PLAYER_CLOSE		00000000000000000000000000000010
	// AiDecision::HEALTH_LOW		00000000000000000000000000000100
	// ...
	// These masks will be OR'd in a DecisionTree to get a unique
	// combination of decisions, that will lead to an AiAction
	//
	enum class AiDecision {
		NONE = 0x00000000,
		PLAYER_VISIBLE = 0x00000001,
		PLAYER_CLOSE = 0x00000002,
		HEALTH_LOW = 0x00000004
	};
	enum class AiAction {
		IDLE,
		MOVE_IN,
		MOVE_OUT,
		ATTACK
	};

	//
	// MapTiles used by the Random Map Generator
	// These TileTypes will be mapped to a part of the Level-Texture
	//
	enum class MapTileTypes {
		Placeholder,
		Portal,

		Top_Default,
		Top_Special,

		Floor_Default,
		Floor_Special_1,
		Floor_Special_2,
		Floor_Special_3,
		Floor_Special_4,
		Floor_Shadow,

		Wall_Top,
		Wall_Left,
		Wall_Right,
		Wall_Bottom,

		Corner_Top_Left,
		Corner_Top_Right,
		Corner_Bottom_Left,
		Corner_Bottom_Right,

		Endblock_Top,
		Endblock_Left,
		Endblock_Right,
		Endblock_Bottom,

		Middle_Block_Horizontal,
		Middle_Block_Vertical,
	};

	//
	// UI Element Attribute
	//
	enum class UiElementAttribute {
		UIE_Idle,
		UIE_Hover,
		UIE_Selected
	};

}

#endif
