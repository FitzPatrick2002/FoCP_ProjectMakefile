#ifndef IMPORTANT_VARIABLES
#define IMPORTANT_VARIABLES
#include <vector>
#include <map>
#include <tuple>
#include <iostream>

class Gate;
typedef unsigned int u_int;

namespace circuitSpace {

	enum class GateEnum {
		AND, OR, NOT, NOR, NAND, XOR, XNOR, UNKNOWN = -1
	};

	/// This vector stores 4 - tuples <a, b, c, name>, where a - input_1, b - input_2, c - output, name - type of gate
	/// The entire idea is based on two corresponding vectors
	/// By corresponding we mean that scheme[i] is treated as the gate from gates[i] Elements from gates[i] can access easily its inputs and output number from scheme[i]
	extern std::vector<std::tuple<int, int, int, GateEnum>> scheme;

	/// Main vector in which we are storing the constructed circuit
	extern std::vector<Gate*> gates;

	/// Input nodes from the input file (line IN 1 2 3 ...) are stored in this vector
	/// It is used in the function readInputFile()
	extern std::vector<int> inputNodeNums;

	/// Nodes read from the circuit file, denoted in the line with OUT 10 20 30  Used in a function readCircuitFile() and later in makeConnections()
	extern std::vector<int> finalOutputGateNums;

	///Pointers to gates from the gates vector that have numbers from finalOutputGateNums vector as output 
	extern std::vector<Gate*> finalOutputGatesPtrs;
}

namespace fileHandling {

	///It is used in the reading of the input fil, the inner vector one stores the pairs (input node number, state on the input) ex. (23, 1), (12, 0) -> on the 23'rd node we have a high state
	extern std::vector<std::vector<std::pair<int, bool>>> inputVector;

	///It says which of the nested vectors in inputVector is being used
	extern int currentInputCombinationCount;

	///FileNames vector maps the switches -i -u -o to the corresponding file names for input, circuit and output file
	extern std::map<std::string, std::string> fileNames;

}
#endif // !IMPORTANT_VARIABLES
