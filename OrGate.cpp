#include "OrGate.h"

OR::OR(Gate* ch_1, Gate* ch_2) : Gate(ch_1, ch_2) {

}

bool OR::getOutputValue() {
	bool input_1 = getInput_1();
	bool input_2 = getInput_2();

	return input_1 || input_2;
}