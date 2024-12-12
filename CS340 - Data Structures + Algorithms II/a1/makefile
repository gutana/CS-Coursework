CXX = gcc
TARGET = assign1albayatm
FLAGS = -lstdc++

all: main

run: all
	./$(TARGET)

main: main.o Stack.o Tree.o
	$(CXX) *.o -o assign1albayatm $(FLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o $(FLAGS)

Stack.o: 
	$(CXX) -c structures/Stack.cpp -o Stack.o $(FLAGS)

Tree.o: 
	$(CXX) -c structures/Tree.cpp -o Tree.o

clean:
	rm -rf *.o $(TARGET) *.exe 