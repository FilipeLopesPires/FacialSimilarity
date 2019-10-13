COMPILER=g++
C++VERSION=-std=c++17
FLAGS=-Wall
BUILD=build/
SRC=src/

.PHONY: fcm generator docs clean

fcm: $(BUILD) $(BUILD)fcm.o $(BUILD)argsParsingFCM.o $(BUILD)model.o
	$(COMPILER) $(C++Version) $(FLAGS) $(BUILD)fcm.o $(BUILD)argsParsingFCM.o $(BUILD)model.o -o fcm

generator: $(BUILD) $(BUILD)generator.o $(BUILD)argsParsingGEN.o $(BUILD)model.o
	$(COMPILER) $(C++Version) $(FLAGS) $(BUILD)generator.o $(BUILD)argsParsingGEN.o $(BUILD)model.o -o generator

$(BUILD)fcm.o: $(SRC)fcm.cpp
	$(COMPILER) $(C++Version) $(FLAGS) -c $(SRC)fcm.cpp -o $(BUILD)fcm.o

$(BUILD)generator.o: $(SRC)generator.cpp
	$(COMPILER) $(C++Version) $(FLAGS) -c $(SRC)generator.cpp -o $(BUILD)generator.o

$(BUILD)argsParsingFCM.o: $(SRC)argsParsingFCM.cpp $(SRC)argsParsing.h
	$(COMPILER) $(C++Version) $(FLAGS) -c $(SRC)argsParsingFCM.cpp -o $(BUILD)argsParsingFCM.o

$(BUILD)argsParsingGEN.o: $(SRC)argsParsingGEN.cpp $(SRC)argsParsing.h
	$(COMPILER) $(C++Version) $(FLAGS) -c $(SRC)argsParsingGEN.cpp -o $(BUILD)argsParsingGEN.o

$(BUILD)model.o: $(SRC)model.cpp $(SRC)model.h
	$(COMPILER) $(C++Version) $(FLAGS) -c $(SRC)model.cpp -o $(BUILD)model.o

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)

docs:
	doxygen ../Doxyfile
