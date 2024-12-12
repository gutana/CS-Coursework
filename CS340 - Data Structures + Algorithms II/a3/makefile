CXX = g++
FLAGS = -lstdc++# -g

BUILD_DIR = build
TARGET = assign3albayatm

DELETE = del
all: main

run: all
	./$(TARGET)

main: main.o
	$(CXX) $(BUILD_DIR)/*.o -o $(TARGET) $(FLAGS)

main.o: main.cpp SortCLI.o 
	$(CXX) -c main.cpp -o $(BUILD_DIR)/main.o $(FLAGS)

SortCLI.o: SortCLI.h SortCLI.cpp algorithms/SortingAlgorithms.h InsertionSort.o MergeSort.o QuickSort.o QuickInsertionSort.o
	$(CXX) -c SortCLI.cpp -o $(BUILD_DIR)/SortCLI.o $(FLAGS)

InsertionSort.o: algorithms/SortingAlgorithms.h algorithms/InsertionSort.cpp
	$(CXX) -c algorithms/InsertionSort.cpp -o $(BUILD_DIR)/InsertionSort.o $(FLAGS)

MergeSort.o: algorithms/SortingAlgorithms.h algorithms/MergeSort.cpp
	$(CXX) -c algorithms/MergeSort.cpp -o $(BUILD_DIR)/MergeSort.o $(FLAGS)

QuickSort.o: algorithms/SortingAlgorithms.h algorithms/QuickSort.cpp
	$(CXX) -c algorithms/QuickSort.cpp -o $(BUILD_DIR)/QuickSort.o $(FLAGS)

QuickInsertionSort.o: algorithms/SortingAlgorithms.h algorithms/QuickInsertionSort.cpp
	$(CXX) -c algorithms/QuickInsertionSort.cpp -o $(BUILD_DIR)/QuickInsertionSort.o $(FLAGS)

clean:
	$(DELETE) -rf $(BUILD_DIR)*.o  $(TARGET) *.exe 