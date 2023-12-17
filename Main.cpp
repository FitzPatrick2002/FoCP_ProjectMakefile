/*! \file*/
#include <regex>
#include <sstream>
#include <fstream>
//#include <filesystem>

#include "ImportantVariables.h"
#include "IncludeAllGates.h"

/// Function called in @see checkCorrectOrderOfArgs() when program is called without switches. It displays help file.

void printHelp(std::string helpFileName) {
	std::ifstream helpFile(helpFileName);

	if (helpFile.is_open()) {
		std::string line;
		while (std::getline(helpFile, line))
			std::cout << line <<"\n";

		helpFile.close();
	}
	else
		throw (("Could not open the file " + helpFileName + " with help instructions\n").c_str());
}

/// Global function that makes sure that arguments passed from the command line are in correct order
/// @param argn is the amount of arguments passed by command line
/// @param params[] is the char* that stores these arguments
/// We have three flags -i -> input file, -u -> circuit file, and exceptional -o -> output file
/// @note if we call the program with switches, -i and -u must occur with file names
/// @note using same switch more than once is forbidden
/// @note after a switch name of file must occur, otherwise function will throw an exception

void checkCorrectOrderOfArgs(int argn, char* params[]) {
	//If program has been called without switches display help file
	if (argn == 1) {
		printHelp("help.txt");
		return;
	}

	std::string str = "";
	for (int i = 1; i < argn - 1; i++) {
		str += params[i];
		str += " ";
	}
	str += params[argn - 1];
	std::cout << "Passed args: " << str << "\n";

	std::vector<std::string> availableFags = { "-i", "-o", "-u" };

	std::string temp = "";
	std::string flag = "";
	bool nextArgIsFileName = false;
	int j = 0;
	for (int i = 0; i < str.length(); i++) {
		j = str.find_first_of({ " " }, i);

		if (j == -1) //No match found -> end of search
			j = str.length();
		temp = str.substr(i, j - i);

		// We have found a flag, the next argument should be the name of a file
		if (std::find(availableFags.begin(), availableFags.end(), temp) != availableFags.end() && nextArgIsFileName == false) {
			flag = temp;

			nextArgIsFileName = true;
		}
		else if (nextArgIsFileName && std::find(availableFags.begin(), availableFags.end(), temp) == availableFags.end()) {
			//Check if user did not give two flags one after another for ex. -i -u (it should be -i input.txt -u circuit.txt)

			if (fileHandling::fileNames.find(flag) == fileHandling::fileNames.end())
				fileHandling::fileNames[flag] = temp;
			else
				throw (("Switch" + flag + " has been used more than once\n").c_str());
			nextArgIsFileName = false;
		}
		else { 
			throw ("Unknown switch was used or order of arguments was incorrect. Available switches: -i \"input.txt\" -o \"output.txt\" -u \"circuit.txt\" \n");
		}

		i = j;
	}

	//We allow a situation win which the output file -o i s not provided 
	//Name for this file will be generated automatically
	//Such situation though is not acceptable for the -i and -u switches
	//Also we shouldn't be able to reuse one switch more than one time

	//In this case wea re missing one of key switches
	if (fileHandling::fileNames.find("-i") == fileHandling::fileNames.end() || fileHandling::fileNames.find("-u") == fileHandling::fileNames.end())
		throw ("One of essential switches -i or -u is missing");

	for (auto& elem : fileHandling::fileNames)
		std::cout << elem.first << "   " << elem.second << "\n";
}

/// Converts std::string type to circuitSpace::GateEnum type
/// ex. "AND" -> AND == 0
/// @note In case the string doesn't match any of the memebers of circuitSpace::GateEnum it returns circuitSpace::GateEnum::UNKNOWN == -1

circuitSpace::GateEnum convertStringToGateNum(std::string str) {

	if (str == "AND")
		return circuitSpace::GateEnum::AND;
	else if (str == "OR")
		return circuitSpace::GateEnum::OR;
	else if (str == "NOT")
		return circuitSpace::GateEnum::NOT;
	else if (str == "NAND")
		return circuitSpace::GateEnum::NAND;
	else if (str == "NOR")
		return circuitSpace::GateEnum::NOR;
	else if (str == "XOR")
		return circuitSpace::GateEnum::XOR;
	else if (str == "XNOR")
		return circuitSpace::GateEnum::XNOR;

	return circuitSpace::GateEnum::UNKNOWN;
}

/// Function that reads the circuit from the circuit file.
/// It constructs the circuitSpace::scheme and sets the output nodes numbers in circuitSpace::finalOutputGateNums and input node numbers in circuitSpace::inputNodeNums

void readCircuitFile() {
	//Check if we actually have the name of the file that we want to open
	if (fileHandling::fileNames.find("-u") != fileHandling::fileNames.end()) {
		//Check if this file even exist
		//if (std::filesystem::exists(fileHandling::fileNames["-u"])) { //Apparently g++ can;t see the <filesystem> and makefile doesn;t work :/
			std::ifstream circuitFile(fileHandling::fileNames["-u"]);

			if (circuitFile.is_open()) {
				//Iterate through the file and get the info about the circuit scheme

				//This regex is supposed to find the 'NAND 1 2 3' kind of stuff
				std::regex reg("([A-Za-z]+?)\\s+?(\\d+?)\\s+?(\\d+?)\\s+?(\\d+?)");
				std::smatch matches;

				std::string line;
				int linesAmount = 0;

				//First of all we want to know 
				// In: 1 6 -> At first I thought theses are irrevelenat. No they are not, it was stupid we need to rewrite the entire thing
				// Out: 3  -> that one is actually important
				//We want htese two to be in the first two lines of the file, we could keep on looking for them among the file but... why bother?

				//Algorithm is simple
				//We want to have two first lines in format specified above
				//Make the first thre non white characters uppercase and check whther they spell correctly IN / OUT
				//Depending which case it is (IN or OUT) use regex similiar like in reading the input.txt file for reading the input and output nodes

				std::getline(circuitFile, line);
				std::regex inReg("[Ii][nN]");
				std::regex outReg("[Oo][Uu][Tt]");
				std::regex numberReg("\\s*?(\\d+?)\\s*?");

				//Here we read the IN 1 2 3 4 5 nodes and the 
				// OUT 6 7 8 nodes
				//If true -> first line is with input nodes
				if (std::regex_search(line, matches, inReg)) {
					//Extract the beginnning of the line. Find the next number and extract it, repeat till we run out of numbers

					while (std::regex_search(line, matches, numberReg)) {
						circuitSpace::inputNodeNums.push_back(std::atoi(matches[1].str().c_str()));
						line = line.substr(matches.position() + matches.length(), line.length());
					}
					//Read the next line which should contain the OUT nodes

					std::getline(circuitFile, line);
					if (std::regex_search(line, matches, outReg)) {
						//line = line.substr(matches.position() + matches.length(), line.length());
						//std::regex_search(line, matches, numberReg);
						//finalOutGateNum = std::atoi(matches[1].str().c_str());

						//The below stuff would have been used in many-output nodes scenario. But I chickend out cause it was just too ugly later in the code
						//I'm back this time I'll have it done...
						while (std::regex_search(line, matches, numberReg)) {
							circuitSpace::finalOutputGateNums.push_back(std::atoi(matches[1].str().c_str()));
							line = line.substr(matches.position() + matches.length(), line.length()); //Something is not okay beacuse substr in the second argument takes the amount of charaters to add to the first index, not the end index... Does it trim automatically
						}
						
					}
					else 
						throw (("There were problems with reading IN / OUT from the " + fileHandling::fileNames["-u"] + " file provided as circuit file\n").c_str());
					
				}
				else {
					//The first line is actually with the OUTput
					//Second with INput
					if (std::regex_search(line, matches, outReg)) {
						//line = line.substr(matches.position() + matches.length(), line.length());
						//std::regex_search(line, matches, numberReg);
						//finalOutGateNum = std::atoi(matches[1].str().c_str());

						while (std::regex_search(line, matches, numberReg)) {
							circuitSpace::finalOutputGateNums.push_back(std::atoi(matches[1].str().c_str()));
							line = line.substr(matches.position() + matches.length(), line.length());
						}
						
					}

					std::getline(circuitFile, line);
					if (std::regex_search(line, matches, inReg)) {
						//line = line.substr(matches.position() + matches.length(), line.length());
						while (std::regex_search(line, matches, numberReg)) {
							circuitSpace::inputNodeNums.push_back(std::atoi(matches[1].str().c_str()));
							line = line.substr(matches.position() + matches.length(), line.length());
						}
					}
					else 
						throw (("There were problems with reading IN / OUT from the " + fileHandling::fileNames["-u"] + " file provided as circuit file\n").c_str());
				
				}

				//Theoritically there shouldn't be any danger but no one knows...
				circuitSpace::finalOutputGateNums.shrink_to_fit();
				circuitSpace::inputNodeNums.shrink_to_fit();
				//Count the number of lines in the file so we know how many inputs to expect
				while (std::getline(circuitFile, line)) {
					linesAmount++;
				}

				//Reset the reading pointer and reset the eofbit flag
				circuitFile.clear(std::ios::eofbit);
				circuitFile.seekg(std::ios::beg);

				circuitSpace::scheme.reserve(linesAmount);

				//Here we read the whole circuit
				//We save the 4 element tuples lke 1 2 3 NAND into circuitSpace::scheme
				//At the same time we make a vector of Gate* pointers the indexes in these vectors correxpond to the same gate
				linesAmount = 0;
				while (std::getline(circuitFile, line)) {
					linesAmount++;
					//If we managed to find a match:
					//Check if gate exists
					//Check for logicall errors (same input as output)

					if (std::regex_search(line, matches, reg)) {
						std::string gateName = matches[1];
						int in_1 = std::atoi(matches[2].str().c_str());
						int in_2 = std::atoi(matches[3].str().c_str());
						int out = std::atoi(matches[4].str().c_str());

						//Make the gateName uppercase
						for (auto& x : gateName)
							x = std::toupper(x);

						//If such a gate exist add it to the circuit vector (but first check for other errors listed above)
						if ((int)convertStringToGateNum(gateName) + 1) {
							//Check if inputs and outputs make sense
							if (in_1 == out || in_2 == out) 
								throw (("Error in reading circuit from file "+ fileHandling::fileNames["-u"] + ". One of inputs is the same as the output. (flip-flops not supperted) Around line " + std::to_string(linesAmount)+"\n").c_str());

							//Assume everything is okay and that connections are not messed up 
							//Add the node...
							circuitSpace::scheme.emplace_back(std::make_tuple(in_1, in_2, out, convertStringToGateNum(gateName)));
						}
						else 
							throw (("Error in reading circuit from file " + fileHandling::fileNames[" - u"] + ". Name of a gate is unknown around line " + std::to_string(linesAmount) + "\n").c_str());
					}
					else {
						//If match hasn't been found then it means that we got a blank line or something like that.
						//It could also mean that one of the lines is wrongly set but we handle that later
					}
				}

				circuitSpace::scheme.shrink_to_fit();
				circuitFile.close();
			}
		//}
		//else {
		//	throw std::exception(("File: " + fileHandling::fileNames["-n"] + " doesn't exist. It has been provided as a circuit file\n").c_str());
		//}
	}
	else {
		throw ("No file name for circuit has been provided.\n");
	}
}

/// Reads the values from the input file. 

void readInputFile() {

	//Name for the input file has been provided, YaY ! ---> Boobs: (.Y.)
	if (fileHandling::fileNames.find("-i") != fileHandling::fileNames.end()) {

		//if (std::filesystem::exists(fileHandling::fileNames["-i"])) {
			std::ifstream inputFile(fileHandling::fileNames["-i"]);

			if (inputFile.is_open()) {
				std::string inputFileLine = "";

				//This regex means that we may have some spcaes -> 12  : 1		 2 : 
				std::regex inputRegex("(\\d+?)\\s*?:\\s*?([0,1])\\s*?");
				std::smatch matches;
				int matchesAmount = 0;

				//There can be many input gates
				//Iterate through the string and omit the inappropriate inputs and count how many appropriate there was
				//If everything is all right proceed if not return false

				//For now an empty vector
				//inputVector.reserve(1);
				//inputVector.emplace_back();


				//Count the number of lines. They denote for how many combinations of input we want to runt the program
				int i = 0;
				int combinationsAmount = 0; //Approxiamtely the amount of lines in the file
				while (std::getline(inputFile, inputFileLine))
					combinationsAmount++;

				//Go back to the beginning of the file
				inputFile.clear(std::ios::eofbit);
				inputFile.seekg(std::ios::beg);

				fileHandling::inputVector.reserve(combinationsAmount);
				for (int j = 0; j < fileHandling::inputVector.capacity(); j++)
					fileHandling::inputVector.emplace_back();

				while (std::getline(inputFile, inputFileLine)) {
					fileHandling::inputVector[i].reserve(circuitSpace::inputNodeNums.size()); //The amount of input wires or whatever...
					while (std::regex_search(inputFileLine, matches, inputRegex)) {
						//First check whether the input node from file actually is an input node (is specified in  line 'IN 23 4...' in the circuit.txt file
						int nodeNum = std::atoi(matches[1].str().c_str());
						int nodeVal = std::atoi(matches[2].str().c_str());
						if (std::find(circuitSpace::inputNodeNums.begin(), circuitSpace::inputNodeNums.end(), nodeNum) != circuitSpace::inputNodeNums.end())
							fileHandling::inputVector[i].emplace_back(nodeNum, nodeVal);
						else {
							std::cout << "Unknown node value specified in the input.txt file\n";
							std::cout << "It hasn't been specified in the circuit.txt file in the line \\'IN...\\'";
							break;
						}

						inputFileLine = inputFileLine.substr(matches.position() + matches.length(), inputFileLine.length());
						matchesAmount++;
					}
					i++;
				}
				fileHandling::inputVector.shrink_to_fit();

				//TO DO: 
				//Add the thing that cheks whether we have enough inputs for our system

				inputFile.close();
				if (fileHandling::inputVector.size() != fileHandling::inputVector.capacity()) 
					throw (("There is a lesser amount of inputs in the input file: " + fileHandling::fileNames["-i"] + " than has been specified in the circuit file" + fileHandling::fileNames["-u"] + "\n").c_str());

			}
		//}
	}

}


/// Constructs the @see gates vector and creates connections between gates.
/// After reading the circuit from the circuit file we have the @see scheme vector ready.
/// We want to construct the @see gates vector in such a manner that the element gates[i] can read it's input nodes numbers from the @see scheme

void makeConnections() {
	int in_1;// = std::get<0>(elem);
	int in_2;// = std::get<1>(elem);
	int out;// = std::get<2>(elem);

	//Create the gates
	circuitSpace::gates.reserve(circuitSpace::scheme.size());
	for (const auto& elem : circuitSpace::scheme) {
		circuitSpace::GateEnum gateType = std::get<3>(elem);

		switch (gateType) {
		case circuitSpace::GateEnum::AND:
			circuitSpace::gates.emplace_back(new AND);
			break;
		case circuitSpace::GateEnum::OR:
			circuitSpace::gates.emplace_back(new OR);
			break;
		case circuitSpace::GateEnum::NOT:
			circuitSpace::gates.emplace_back(new NOT);
			break;
		case circuitSpace::GateEnum::NOR:
			circuitSpace::gates.emplace_back(new NOR);
			break;
		case circuitSpace::GateEnum::NAND:
			circuitSpace::gates.emplace_back(new NAND);
			break;
		case circuitSpace::GateEnum::XOR:
			circuitSpace::gates.emplace_back(new XOR);
			break;
		case circuitSpace::GateEnum::XNOR:
			circuitSpace::gates.emplace_back(new XNOR);
			break;
		default:
			throw ("Function makeConnections() could not read a gate type from the scheme vector\n");
			//break; //Is it needed if we throw?
		}
	}
	//Now create connections
	//We can do it as such because they gates[i] corresponds to scheme[i] (I do hope it does)
	for (int i = 0; i < circuitSpace::gates.size(); i++) {
		in_1 = std::get<0>(circuitSpace::scheme[i]);
		in_2 = std::get<1>(circuitSpace::scheme[i]);

		//We look for the final output gate so as to obtain a pointer to it
		//if (finalOutGateNum == std::get<2>(scheme[i]))
			//finalOutputGate = gates[i];

		//In case we had many output nodes it would have worked like that
		if (std::find(circuitSpace::finalOutputGateNums.begin(), circuitSpace::finalOutputGateNums.end(), std::get<2>(circuitSpace::scheme[i])) != circuitSpace::finalOutputGateNums.end()) {
			circuitSpace::finalOutputGatesPtrs.push_back(circuitSpace::gates[i]);
		}

		for (int j = 0; j < circuitSpace::scheme.size(); j++) {
			out = std::get<2>(circuitSpace::scheme[j]);

			//Apparently NOR gates work in such manner that they still require two inputs (though the same)
			//But after all they don't really work in this case per seeee
			if (in_1 == out) {

				in_1 == -1;
				circuitSpace::gates[i]->setChild_1(circuitSpace::gates[j]);
			}
			if (in_2 == out) {

				in_2 = -1;
				circuitSpace::gates[i]->setChild_2(circuitSpace::gates[j]);
			}

			if (in_1 == -1 && in_2 == -1)
				break;
		}
	}
}

void writeToOutputFile() {
	std::ofstream outputFile;

	//Output file name has been provided
	if (fileHandling::fileNames.find("-o") == fileHandling::fileNames.end()) {
		//And not only provided but also exists!
		//if (std::filesystem::exists(fileHandling::fileNames["-o"])) //There should be != in the condition without these comments, but g++ can't see the <filesystem>
		//	outputFile.open(fileHandling::fileNames["-o"]);
		//else
			outputFile.open("output.txt");
	}
	else
		outputFile.open(fileHandling::fileNames["-o"]);

	if (outputFile.is_open()) {

		for (auto& currentCombination : fileHandling::inputVector) {

			//Give the output for each 'OUT' node (there could have been many)
			outputFile << "IN:  ";
			for (auto elem : currentCombination) {
				outputFile << elem.first << ":" << elem.second << "  ";
				std::cout << elem.first << ":" << elem.second << "  ";
			}
			//outputFile << "OUT: " << finalOutputGate->getOutputValue() << "\n";;

			// The entire idea of two corresponding vectors is backfiring at me :(
			
			outputFile << "OUT: ";
			std::cout << "OUT: ";

			int numInScheme = 0;
			for (auto& ptr : circuitSpace::finalOutputGatesPtrs) {
				numInScheme = 0;

				while (circuitSpace::gates[numInScheme] != ptr)
					numInScheme++;

				outputFile << std::get<2>(circuitSpace::scheme[numInScheme]) << ":" << ptr->getOutputValue() << "  ";
				std::cout << std::get<2>(circuitSpace::scheme[numInScheme]) << ":" << ptr->getOutputValue() << "  ";
			}
			outputFile << "\n";
			std::cout << "\n";

			fileHandling::currentInputCombinationCount++;
		}

	}
	else {
		throw "Could not open provided output file\n";
	}

	outputFile.close();

	//Iterate through the input combinations
	/*for (auto& currentCombination : inputVector) {

		//Give the output for each 'OUT' node (ther could have been many)
		std::cout << "IN:\n";
		for (auto elem : currentCombination) {
			std::cout << elem.x << ":" << elem.y << " ";
		}
		std::cout << "\n";

		std::cout << "OUT:" << finalOutputGate->getOutputValue() << "\n\n";;
		currentInputCombinationCount++;
	}*/


}

int main(int argn, char* params[]) {

	try {
		checkCorrectOrderOfArgs(argn, params);
		readCircuitFile();
		readInputFile();
		makeConnections();
		writeToOutputFile();
	}
	catch (const char* e) {
		std::cout << "Exception occured:\n";
		std::cout << e << "\n";
	}

	/*Gate* outputGate;

	circuitSpace::gates.push_back(new NOT);
	circuitSpace::gates.push_back(new AND);
	circuitSpace::gates.push_back(new OR);
	circuitSpace::gates.push_back(new OR);
	circuitSpace::gates.push_back(new NOT);
	circuitSpace::gates.push_back(new NOT);

	outputGate = circuitSpace::gates[0];
	outputGate->setChild_1(circuitSpace::gates[1]);
	circuitSpace::gates[1]->setChild_1(circuitSpace::gates[2]);
	circuitSpace::gates[1]->setChild_2(circuitSpace::gates[3]);
	circuitSpace::gates[2]->setChild_1(circuitSpace::gates[4]);
	circuitSpace::gates[3]->setChild_2(circuitSpace::gates[5]);


	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(7, 7, 8, circuitSpace::GateEnum::NOT));
	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(5, 6, 7, circuitSpace::GateEnum::AND));
	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(3, 1, 5, circuitSpace::GateEnum::OR));
	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(1, 4, 6, circuitSpace::GateEnum::OR));
	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(0, 0, 3, circuitSpace::GateEnum::NOT));
	circuitSpace::scheme.push_back(std::tuple<int, int, int, circuitSpace::GateEnum>(2, 2, 4, circuitSpace::GateEnum::NOT));

	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[0].push_back(std::pair<int, bool>(0, 0));
	fileHandling::inputVector[0].push_back(std::pair<int, bool>(1, 0));
	fileHandling::inputVector[0].push_back(std::pair<int, bool>(2, 0));
	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[1].push_back(std::pair<int, bool>(0, 0));
	fileHandling::inputVector[1].push_back(std::pair<int, bool>(1, 0));
	fileHandling::inputVector[1].push_back(std::pair<int, bool>(2, 1));
	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[2].push_back(std::pair<int, bool>(0, 1));
	fileHandling::inputVector[2].push_back(std::pair<int, bool>(1, 0));
	fileHandling::inputVector[2].push_back(std::pair<int, bool>(2, 0));
	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[3].push_back(std::pair<int, bool>(0, 1));
	fileHandling::inputVector[3].push_back(std::pair<int, bool>(1, 0));
	fileHandling::inputVector[3].push_back(std::pair<int, bool>(2, 1));
	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[4].push_back(std::pair<int, bool>(0, 0));
	fileHandling::inputVector[4].push_back(std::pair<int, bool>(1, 1));
	fileHandling::inputVector[4].push_back(std::pair<int, bool>(2, 0));
	fileHandling::inputVector.push_back(std::vector <std::pair<int, bool>>());

	fileHandling::inputVector[5].push_back(std::pair<int, bool>(0, 1));
	fileHandling::inputVector[5].push_back(std::pair<int, bool>(1, 1));
	fileHandling::inputVector[5].push_back(std::pair<int, bool>(2, 0));

	for (int i = 0; i < 6; i++) {
		fileHandling::currentInputCombinationCount = i;
		for (auto& p : fileHandling::inputVector[i]) {
			std::cout << p.first << ":" << p.second << " | ";
		}
		std::cout << " -> " << outputGate->getOutputValue() << "\n";
	}*/

	return 0;
}