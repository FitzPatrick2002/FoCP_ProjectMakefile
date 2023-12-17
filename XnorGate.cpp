#include "XnorGate.h"

XNOR::XNOR(Gate* ch_1, Gate* ch_2) : Gate(ch_1, ch_2) {

}

bool XNOR::getOutputValue() {
	bool input_1 = getInput_1();
	bool input_2 = getInput_2();

	return !((input_1 && !input_2) || (!input_1 && input_2));
}