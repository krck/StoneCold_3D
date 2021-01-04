
#ifndef STONECOLD_DECISIONTREE_H
#define STONECOLD_DECISIONTREE_H

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "EnumHelper.hpp"
#include "Types.hpp"

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	//
	// AI TREES EXAMPLE   
	//				Melee Enemy                                       Ranged Enemy
	//                 root                                               root
	//                  |                                                  |
	//                  V                                                  V
	//      ---- PLAYER_VISIBLE ----                            ---- PLAYER_VISIBLE ----
	//   No |                      | Yes                     No |                      | Yes
	//      V                      V                            V                      V
	//    IDLE            ---- PLAYER_CLOSE ----               IDLE           ---- PLAYER_CLOSE ----
	//                 No |                    | Yes                       No |                    | Yes
	//                    V                    V                              V                    V
	//              - HEALTH_LOW -       - HEALTH_LOW -                    ATTACK              MOVE_OUT            
	//           No |        Yes |    No |            | Yes
	//              V            V       V            V
	//           MOVE_IN        IDLE   ATTACK      MOVE_OUT
	//
	// A tree string will be parsed, traversed and any final node (AiAction) will be saved
	// with a bit-mask based on all previous (AiDecision) nodes. Every decision has a unique
	// bit-flag in a 32-bit mask and if the decision is true (YES-Path) this flag will be 
	// saved as 1, else (NO-Path) it will be 0. Examples based on the melee tree above:
	//
	// ENUM EXAMPLE
	// AiDecision::PLAYER_VISIBLE	00000000000000000000000000000001
	// AiDecision::PLAYER_CLOSE		00000000000000000000000000000010
	// AiDecision::HEALTH_LOW		00000000000000000000000000000100
	// AiAction::IDLE				0
	// AiAction::MOVE_IN			1
	// AiAction::MOVE_OUT			2
	// AiAction::ATTACK				3
	//
	// OUTPUT EXAMPLE
	// Action: 2   Decisions: 00000000000000000000000000000111
	// Action: 3   Decisions: 00000000000000000000000000000011
	// Action: 0   Decisions: 00000000000000000000000000000101
	// Action: 1   Decisions: 00000000000000000000000000000001
	// Action: 0   Decisions: 00000000000000000000000000000000
	//
	// Each final bit-mask (OUTPUT EXAMPLE Decisions) will be saved and can be queried
	// to receive a AiAction based on all corresponding AiDecisions from the AI-tree
	//

	class DecisionTee {
	public:
		DecisionTee(std::string ai);

		std::unordered_map<bitMask32, AiAction> GetActions() const { return _actions; }

	private:
		void CreateNodesFromString(std::string str, bitMask32 decisions);

		//
		// Parse the string starting at a "(" to get the index of its
		// corresponding ")". This determens the full scope of a Node
		//
		size_t GetScopeEnd(std::string str);

	private:
		std::unordered_map<bitMask32, AiAction> _actions;
	};

}

#endif
