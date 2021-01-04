
#include "DecisionTee.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Engine;


DecisionTee::DecisionTee(std::string ai) : _actions(std::unordered_map<bitMask32, AiAction>()) {
	try {
		// Check the local copy and remove all whitespace from the string (isspace checks for ' ', '\t', '\n')
		ai.erase(std::remove_if(ai.begin(), ai.end(), [](unsigned char x) {return std::isspace(x); }), ai.end());
		if (ai.empty())
			return;

		// Start the recursive parsing / node creation
		bitMask32 decisions = 0;
		CreateNodesFromString(ai, decisions);
	}
	catch (const std::exception & ex) {
		std::cout << "Failed to create DecisionTee:\n" << ex.what() << "\n\n" << ai << std::endl;
	}
}


void DecisionTee::CreateNodesFromString(std::string str, bitMask32 decisions) {
	// Every Node in the string must be wrapped in parentheses "(node)"
	if (str.empty() || str.front() != '(' || str.back() != ')')
		return;

	// Chop off the "(" and ")" in the front and back and check if the string
	// still has parentheses. If so "DecisionNode", otherwise "ActionNode"
	str = str.substr(1, str.length() - 2);
	auto dataEndIdx = std::min(str.find_first_of('('), str.find_first_of(')'));
	if (dataEndIdx != std::string::npos) {
		auto condition = EnumHelper::GetAiDecisionFromString(str.substr(0, dataEndIdx));
		auto strRest = str.substr(dataEndIdx, str.length() - dataEndIdx);

		// If its not the last char and the next is '(' then find its complement ')' to get the first childs scope
		auto childSplit = (strRest.length() > 1 && strRest[0] == '(')
			? GetScopeEnd(strRest)
			: (strRest.length() - 1);

		if (childSplit != -1) {
			// Add the Component Flag to the mask (OR bitmask with the current value)
			auto trueCondition = (decisions | static_cast<bitMask32>(condition));

			// Call recursively for the left and right node with eighter a 1 or 0 bit
			CreateNodesFromString(strRest.substr(0, childSplit + 1), trueCondition);
			CreateNodesFromString(strRest.substr(childSplit + 1), decisions);
		}
	}
	else {
		// New ActionNode with all previous decisions as bitmask (1 if true-path, 0 if false-path)
		_actions.insert({ decisions, EnumHelper::GetAiActionFromString(str) });
	}
}


size_t DecisionTee::GetScopeEnd(std::string str) {
	int count = 0;
	// Increment for every ( and decrement for every ) to get the scope of the first node
	for (int i = 0; i < str.length(); i++) {
		count += (str[i] == '(');
		count -= (str[i] == ')');
		if (count == 0 && i != 0)
			return i;
	}
	return (str.length() - 1);
}
