#ifndef NOT_GATE
#define NOT_GATE
#include "Gate.h"

/// Class that simulates the NOT gate.
/// Normally NOT gate has only one input but in this case it inherits from Gate and has two fields. 
/// However they cannot be set to different values.

class NOT : public Gate {
public:

	/// It takes only one parameter in contrary to other classes., both children are set to the same value
	NOT(Gate* ch_1 = nullptr);

	///@return ! getInput_1()
	virtual bool getOutputValue();

	/// Since NOT should have only one input it sets both children to the same value
	void setChild_1(Gate*& ch);

	/// Disabled in this class
	void setChild_2(Gate*& ch);
};

#endif