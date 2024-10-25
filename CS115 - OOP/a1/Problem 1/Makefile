main: weather.o main.o
	g++ -g main.o weather.o -o main

weather.o: weather.cpp weather.h
	g++ -c weather.cpp

main.o: main.cpp 
	g++ -c main.cpp

clean:
	del *.o