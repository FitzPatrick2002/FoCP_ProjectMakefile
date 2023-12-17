#include "ImportantVariables.h"

namespace circuitSpace {

	std::vector<std::tuple<int, int, int, GateEnum>> scheme;

	std::vector<Gate*> gates;

	std::vector<int> inputNodeNums;

	std::vector<int> finalOutputGateNums;

	std::vector<Gate*> finalOutputGatesPtrs;
}

namespace fileHandling {
	std::vector<std::vector<std::pair<int, bool>>> inputVector;

	int currentInputCombinationCount;

	std::map<std::string, std::string> fileNames;
}

