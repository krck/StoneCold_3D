
#ifndef STONECOLD_ENUMHELPER_H
#define STONECOLD_ENUMHELPER_H

#include <string>
#include "Enums.hpp"

namespace StoneCold::Base {

	//
	// EnumHelper
	// Put all "Enum -> String" and "String -> Enum" translations here
	//
	class EnumHelper {
		static AiDecision GetAiDecisionFromString(const std::string& str) {
			if (str == "AiDecision::PLAYER_VISIBLE") return AiDecision::PLAYER_VISIBLE;
			else if (str == "AiDecision::PLAYER_CLOSE") return AiDecision::PLAYER_CLOSE;
			else if (str == "AiDecision::HEALTH_LOW") return AiDecision::HEALTH_LOW;
			else return AiDecision::NONE;
		}

		static AiAction GetAiActionFromString(const std::string& str) {
			if (str == "AiAction::MOVE_IN") return AiAction::MOVE_IN;
			else if (str == "AiAction::MOVE_OUT") return AiAction::MOVE_OUT;
			else if (str == "AiAction::ATTACK") return AiAction::ATTACK;
			else return AiAction::IDLE;
		}
	};

}

#endif
