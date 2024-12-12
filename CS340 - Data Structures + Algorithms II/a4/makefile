CXX = cc
FLAGS = -lstdc++ -lm -std=c++17# -Wconversion

BUILD_DIR = build
TARGET = assign4albayatm

DELETE = rm
all: main

run: all
	./$(TARGET)

main: main.o
	$(CXX) $(BUILD_DIR)/*.o -o $(TARGET) $(FLAGS)

main.o: main.cpp SearchCLI.o
	$(CXX) -c main.cpp -o $(BUILD_DIR)/main.o $(FLAGS)

SearchCLI.o: SearchCLI.h SearchCLI.cpp 8PuzzleSearch.o
	$(CXX) -c SearchCLI.cpp -o $(BUILD_DIR)/SearchCLI.o $(FLAGS)

8PuzzleSearch.o: algorithms/8PuzzleSearch.h algorithms/8PuzzleSearch.cpp Stack.o PuzzleState.o MinMaxHeap.o
	$(CXX) -c algorithms/8PuzzleSearch.cpp -o $(BUILD_DIR)/8PuzzleSearch.o $(FLAGS)

PuzzleState.o: algorithms/PuzzleState.h algorithms/PuzzleState.cpp
	$(CXX) -c algorithms/PuzzleState.cpp -o $(BUILD_DIR)/PuzzleState.o $(FLAGS)

MinMaxHeap.o: algorithms/MinMaxHeap.h algorithms/MinMaxHeap.cpp
	$(CXX) -c algorithms/MinMaxHeap.cpp -o $(BUILD_DIR)/MinMaxHeap.o $(FLAGS)

Stack.o: algorithms/Stack.h algorithms/Stack.cpp
	$(CXX) -c algorithms/Stack.cpp -o $(BUILD_DIR)/Stack.o $(FLAGS)

clean:
	$(DELETE) -rf $(BUILD_DIR)/*.o  $(TARGET) *.exe 