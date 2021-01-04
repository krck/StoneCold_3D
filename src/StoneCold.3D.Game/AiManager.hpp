
#ifndef STONECOLD_AIMANAGER_H
#define STONECOLD_AIMANAGER_H

#include "DecisionTee.hpp"

namespace StoneCold::Game {

using namespace StoneCold::Base;

class AiManager {
public:
	AiManager();
	AiManager(const AiManager&) = delete;
	AiManager& operator=(const AiManager&) = delete;

	//
	// Creates a DecisionTree that will parse the AI-String and store
	// every decision and following action with the specified AI-Name
	//
	void AddAiBehaviour(const std::string& aiName, const std::string& aiString);

	//
	// Get all decisions and following actions of any pared AI-String by name
	//
	std::unordered_map<bitMask32, AiAction>* GetAiBehaviour(const std::string& aiName);

	~AiManager() = default;

private:
	std::unordered_map<std::string, std::unordered_map<bitMask32, AiAction>> _aiBehaviours;
};

}

#endif
