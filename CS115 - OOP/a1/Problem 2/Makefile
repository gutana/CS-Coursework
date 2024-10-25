# Makefile
#
# Mohammad Al-Bayati
# ID: 200339106
# CS115 Fall 2021

main: Weather.o Temperature.o Rainfall.o main.o
	g++ main.o Weather.o Rainfall.o Temperature.o -o main

Weather.o: Weather.cpp Weather.h Rainfall.h Temperature.h
	g++ -c Weather.cpp

Rainfall.o: Rainfall.cpp Rainfall.h
	g++ -c Rainfall.cpp

Temperature.o: Temperature.cpp Temperature.h
	g++ -c Temperature.cpp

main.o: main.cpp 
	g++ -c main.cpp

clean:
	del *.o