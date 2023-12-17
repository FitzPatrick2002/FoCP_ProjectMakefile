#include "NotGate.h"

NOT::NOT(Gate* ch_1) : Gate(ch_1, ch_1) {

}

bool NOT::getOutputValue() {
	bool input_1 = getInput_1();

	return !input_1;
}

void NOT::setChild_1(Gate*& ch) {
	mChild_1 = ch;
	mChild_2 = ch;
}

void NOT::setChild_2(Gate*& ch)  {
	//Do nothing
}