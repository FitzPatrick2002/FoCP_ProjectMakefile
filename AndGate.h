#ifndef AND_GATE
#define AND_GATE
#include "Gate.h"

/// Class that simulates the AND logic gate. 
/// @note Only thing it does is overwriting of Gate::getOutputValue()

class AND : public Gate {
public:

	/// Sets the values of mChild_1 and mChild_2
	AND(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	/// @return getInput_1() && getInput_2()
	virtual bool getOutputValue();
};
#endif //!AND_GATE
