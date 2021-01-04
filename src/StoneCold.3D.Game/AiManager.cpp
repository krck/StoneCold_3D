
#include "AiManager.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Game;
using namespace StoneCold::Engine;


AiManager::AiManager() : _aiBehaviours(std::unordered_map<std::string, std::unordered_map<bitMask32, AiAction>>()) { }


void AiManager::AddAiBehaviour(const std::string& aiName, const std::string& aiString) {
	// Create a DecisionTree that will parse the AI-String
	auto dt = DecisionTee(aiString);
	_aiBehaviours.insert({ aiName, dt.GetActions() });
}


std::unordered_map<bitMask32, AiAction>* AiManager::GetAiBehaviour(const std::string& aiName) {
	return (_aiBehaviours.find(aiName) != _aiBehaviours.end())
		? &_aiBehaviours[aiName]
		: nullptr;
}
