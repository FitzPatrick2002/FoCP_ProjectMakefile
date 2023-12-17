#include "Gate.h"
#include "ImportantVariables.h"

Gate::Gate(Gate* ch_1, Gate* ch_2) : mChild_1(ch_1), mChild_2(ch_2) {

}

Gate::~Gate() {

}

/// A recursive function that obtains the state at input 1
/// Algorith mthat it follows:
/// 1. Check if the child1 is a nullptr
/// 2. If it is not a nullptr return the value provided at the ouput of this child
/// 3. If it is a nullptr 
///   3.1. Establish which element of the *gates* vector it is
///   3.2. From the *scheme* vector get the number of the input node
///   3.3. Parse the @see inputVector (one responsible for storing combinations of input) in search for this node
///   3.4. retern it's value

bool Gate::getInput_1() {
	if (mChild_1 == nullptr) {
		int i = 0;
		while (circuitSpace::gates[i] != this)
			i++;

		int inputNodeNum = std::get<0>(circuitSpace::scheme[i]);
		std::pair<int, bool> theReturnValue;
		for (auto elem : fileHandling::inputVector[fileHandling::currentInputCombinationCount]) {
			if (elem.first == inputNodeNum)
				return elem.second;
		}

		std::cout << "Something went terribly wrong when returning from the inputVector, line : " << __LINE__ << "\n";
		//return outsideInput_1;
	}

	return mChild_1->getOutputValue();
}

/// It works similiarily to @see getInput_2() 
bool Gate::getInput_2() {
	if (mChild_2 == nullptr) {
		int i = 0;
		while (circuitSpace::gates[i] != this)
			i++;

		int inputNodeNum = std::get<1>(circuitSpace::scheme[i]);
		std::pair<int, bool> theReturnValue;
		for (auto elem : fileHandling::inputVector[fileHandling::currentInputCombinationCount]) {
			if (elem.first == inputNodeNum)
				return elem.second;
		}

		std::cout << "Something went terribly wrong when returning from the inputVector, line : " << __LINE__ << "\n";
	}

	return mChild_2->getOutputValue();
}

//Getters and setters

void Gate::setChild_1(Gate*& ch) {
	mChild_1 = ch;
}

void Gate::setChild_2(Gate*& ch) {
	mChild_2 = ch;
}

Gate* Gate::getPointerChild_1() {
	return mChild_1;
}

Gate* Gate::getPointerChild_2() {
	return mChild_2;
}

