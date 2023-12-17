#pragma once
#ifndef NOR_GATE
#define NOR_GATE
#include "Gate.h"

/// Class that simulates the NOR logic gate. 
/// @note Only thing it does is the overwriting of Gate::getOutputValue()

class NOR : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	NOR(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return !(getInput_1() || getInput_2())
	virtual bool getOutputValue();
};
#endif
