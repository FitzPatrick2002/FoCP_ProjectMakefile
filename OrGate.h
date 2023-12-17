#ifndef OR_GATE
#define OR_GATE
#include "Gate.h"


/// Class that simulates the OR logic gate. 
/// @note Only thing it does is the overwriting of Gate::getOutputValue()

class OR : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	OR(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return getInput_1() || getInput_2()
	virtual bool getOutputValue();
};
#endif