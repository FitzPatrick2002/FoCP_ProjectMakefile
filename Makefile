program: Main.o Gate.o ImportantVariables.o AndGate.o OrGate.o NotGate.o NandGate.o NorGate.o XorGate.o XnorGate.o 
	g++ Main.o Gate.o ImportantVariables.o AndGate.o OrGate.o NotGate.o NandGate.o NorGate.o XorGate.o XnorGate.o -o program

ImportantVariables.o: ImportantVariables.cpp ImportantVariables.h
	g++ -c ImportantVariables.cpp
	
Gate.o: Gate.cpp Gate.h
	g++ -c Gate.cpp
	
AndGate.o: AndGate.cpp AndGate.h
	g++ -c AndGate.cpp
	
OrGate.o: OrGate.cpp OrGate.h
	g++ -c OrGate.cpp
	
NotGate.o: NotGate.cpp NotGate.h
	g++ -c NotGate.cpp
	
NandGate.o: NandGate.cpp NandGate.h
	g++ -c NandGate.cpp
	
NorGate.o: NorGate.cpp NorGate.h
	g++ -c NorGate.cpp
	
XorGate.o: XorGate.cpp XorGate.h
	g++ -c XorGate.cpp
	
XnorGate.o: XnorGate.cpp XnorGate.h
	g++ -c XnorGate.cpp

Main.o: Main.cpp
	g++ -c Main.cpp

clean: 
	rm *.o program