#ifndef NAND_GATE
#define NAND_GATE
#include "Gate.h"

/// Class that simulates the NAND logic gate. 
/// @note Only thing it does is the overwriting of @see Gate::getOutputValue()

class NAND : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	NAND(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return !(getInput_1() && getInput_2())
	virtual bool getOutputValue();
};

#endif