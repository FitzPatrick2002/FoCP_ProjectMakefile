#ifndef XNOR_GATE
#define XNOR_GATE
#include "Gate.h"

/// Class that simulates the XNOR logic gate. 
/// @note Only thing it does is overwriting of Gate::getOutputValue()

class XNOR : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	XNOR(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return  !((getInput_1() && ! getInput_2()) || (! getInput_1() && getInput_2()))
	virtual bool getOutputValue();

};

#endif // !XNOR_GATE
