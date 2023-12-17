#ifndef XOR_GATE
#define XOR_GATE
#include "Gate.h"

/// Class that simulates the XOR logic gate. 
/// @note Only thing it does is the overwriting of Gate::getOutputValue()

class XOR : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	XOR(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return (getInput_1() && !getInput_2()) && (!getInput_1() && getInput_2())
	virtual bool getOutputValue();

};

#endif