#ifndef GATECLASS
#define GATECLASS

class Gate {
protected:
	Gate* mChild_1;
	Gate* mChild_2;

public:
	/// By default we set children of the gate to be nullpointers
	/// Apparently Gate() doesn't do anything special...

	Gate(Gate* ch_1 = nullptr, Gate* ch_2 = nullptr);

	virtual ~Gate();

	/// It returns the final logic value that will be at the output of the gate after performing logic opeartion specified by the gate.
	/// It is a purely virtual function.
	/// Implementation differs for different logic gates, however it follows a pattern:
	/// 1. Get the values on both inputs of the gate
	/// 2. Return the logic operation specified by the logic gate ex. for AND: (v1 && v2) <- returned value, where v1 and v2 are states at the input 1 and 2 of the gate
	/// @see getInput_1() <- In this function is specified how we obtain the state on the v1
	/// @see getInput_2() <- In this function is specified how we obtain the state on the v2
	/// @note It is a recursive routine
	/// @see circuitSpace::scheme (in the ImportanVariables.h)
	virtual bool getOutputValue() = 0;

	/// It returns the state at the 1'st input of the logic gate
	/// @note This function uses the mpCircuitVector field. 
	/// @warning without providing appropriate mpCircuitVector it will not work
	/// @note It is a recursive routine
	bool getInput_1();
	

	bool getInput_2();

	//Setters and getters

	/// It is a setter for the mChild_1 field
	/// @param ch is a pointer *passed by reference* **I'm not sure whether it is necessary or not...**
	void setChild_1(Gate*& ch);

	void setChild_2(Gate*& ch);

	Gate* getPointerChild_1();

	Gate* getPointerChild_2();
};

#endif 