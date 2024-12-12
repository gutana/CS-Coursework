CXX = gcc
TARGET = assign2albayatm
FLAGS = -lstdc++ -g
TESTDATA = 87 32 5 68 20 45 13 77 50 26 36 62 18 53 9 1 F


all: main

run: all
	./$(TARGET)

test: all
	echo $(TESTDATA) | $(TARGET)

main: main.o
	$(CXX) *.o -o $(TARGET) $(FLAGS)

main.o: main.cpp MinMaxHeap.o MinMaxHeapCLI.o
	$(CXX) -c main.cpp -o main.o $(FLAGS)

MinMaxHeapCLI.o: MinMaxHeapCLI.h MinMaxHeapCLI.cpp structures/MinMaxHeap.h
	$(CXX) -c MinMaxHeapCLI.cpp -o MinMaxHeapCLI.o $(FLAGS)

MinMaxHeap.o: structures/MinMaxHeap.cpp structures/MinMaxHeap.h
	$(CXX) -c structures/MinMaxHeap.cpp -o MinMaxHeap.o $(FLAGS)

clean:
	del -rf *.o $(TARGET) *.exe 